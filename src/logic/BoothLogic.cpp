//
// Created by clemens on 21.01.19.
//

#include "BoothLogic.h"
#include <tools/blocking_reader.h>
#include <unistd.h>
#include <linux/reboot.h>
#include <sys/reboot.h>

using namespace std;
using namespace selfomat::logic;
using namespace selfomat::camera;

/**
 * Looks for arduinos in /dev and returns all found ones
 * @return
 */
vector<boost::filesystem::path> BoothLogic::findArduinos() {
    boost::filesystem::path devPath("/dev/");

    vector<boost::filesystem::path> foundArduinos;
    for (auto &e : boost::make_iterator_range(boost::filesystem::directory_iterator(devPath))) {
        if (boost::starts_with(e.path().string(), button_port))
            foundArduinos.push_back(e.path());
    }
    return foundArduinos;
}

/**
 * Connects the button arduino
 * @param serialPath
 * @return true on success
 */
bool BoothLogic::connectButton(boost::filesystem::path serialPath) {
    // TODO: Check if we really connected to the button and not some other serial device and return a status
    try {
        button_serial_port.open(serialPath.string());
        button_serial_port.set_option(boost::asio::serial_port_base::baud_rate(38400));
        if (disable_watchdog)
            button_serial_port.write_some(boost::asio::buffer("!", 1));
        else
            button_serial_port.write_some(boost::asio::buffer("?", 1));
        button_serial_port.write_some(boost::asio::buffer(".", 1));
    } catch (std::exception const &e) {
        cerr << "Error opening button on port " << serialPath << ". Reason was: " << e.what() << endl;
        return false;
    }
    return true;
}


bool BoothLogic::connectToSerial(boost::filesystem::path serialPath) {
    // TODO: Check if we really connected to the button and not some other serial device and return a status
    try {
        tmp_serial_port.open(serialPath.string());
        tmp_serial_port.set_option(boost::asio::serial_port_base::baud_rate(38400));
        tmp_serial_port.write_some(boost::asio::buffer("i", 1));

        cout << "Waiting for identification" << endl;
        char c;

        blocking_reader reader(tmp_serial_port, 3000);
        if (reader.read_char(c)) {
            cout << "Got a " << c << endl;
            if (c == 'b') {
                cout << "Found the button" << endl;
                return connectButton(serialPath);
            }
            cout << "Unknown identification: " << c << endl;
        } else {
            cout << "No identification received" << endl;
        }
        tmp_serial_port.close();
    } catch (std::exception const &e) {
        cerr << "Error opening button on port " << serialPath << ". Reason was: " << e.what() << endl;
        return false;
    }
    return true;
}


bool BoothLogic::start() {

#ifdef USE_SPI
    cout << "we have SPI!!!" << endl;

    wiringPiSetup();
    wiringPiSetupGpio();
    wiringPiSPISetup(0, 5000);
    pinMode(PIN_SS, OUTPUT);

    digitalWrite(PIN_SS, HIGH);
#endif

    gui->logDebug("Starting Logic");

    if (has_button) {
        gui->logInfo("Seraching for connected Arduinos");


        auto foundArduinos = findArduinos();
        if (foundArduinos.empty()) {
            gui->logError("Error: No arduinos found :-(");
        }

        for (auto path : foundArduinos) {
            std::stringstream sstr;
            sstr << "Connecting arduino at: " << path << endl;
            gui->logInfo(sstr.str());

            auto success = connectToSerial(path);
            if (success) {
                gui->logInfo("Arduino connected successfully!");
            } else {
                gui->logError("Error connecting Arduino");
            }
        }

        if (button_serial_port.is_open()) {
            cout << "Found button" << endl;
        } else {
            cout << "Did not find a button" << endl;
        }
    }

    if (showAgreement) {
        gui->showAgreement();
        if (button_serial_port.is_open())
            button_serial_port.write_some(boost::asio::buffer("a", 1));
    }

    gui->logDebug("Initializing Image Processor");
    if (!imageProcessor.start())
        return false;

    gui->logDebug("Starting Printer");
    if (!printerManager.start())
        return false;

    // Start the threads
    isRunning = true;
    ioThreadHandle = boost::thread(boost::bind(&BoothLogic::ioThread, this));
    logicThreadHandle = boost::thread(boost::bind(&BoothLogic::logicThread, this));
    cameraThreadHandle = boost::thread(boost::bind(&BoothLogic::cameraThread, this));
    printThreadHandle = boost::thread(boost::bind(&BoothLogic::printerThread, this));

    return true;
}

void BoothLogic::triggerFlash() {
    if(!flashEnabled)
        return;

#ifdef USE_SPI
    digitalWrite(PIN_SS, LOW);
    flash_struct flash;
    flash.delay = flashDelayMicros/100;
    flash.duration = flashDurationMicros/100;
    flash.brightness=(int)(255.0f*flashBrightness);
    flash.fade = (int)(255.0f*flashFade);
    wiringPiSPIDataRW(0, (unsigned char*)&flash, 6);
    digitalWrite(PIN_SS, HIGH);
#endif
}

void BoothLogic::stop() {
    std::cout << "stopping logic" << std::endl;
    isRunning = false;

    writeSettings();

    if (button_serial_port.is_open())
        button_serial_port.close();

    gui->stop();

    imageProcessor.stop();

    cout << "waiting for logic" << endl;
    logicThreadHandle.join();
    cout << "waiting for io" << endl;
    ioThreadHandle.join();
    cout << "waiting for cam" << endl;
    cameraThreadHandle.join();
    cout << "waiting for print" << endl;
    printThreadHandle.join();
}

void BoothLogic::cameraThread() {
    gui->logDebug("Starting Camera Thread");
    gui->initialized();
    while (isRunning) {
        if (camera->getState() != CameraState::STATE_WORKING) {
            // Camera is not working, try to get it working
            gui->logDebug("Starting Camera");
            CameraStartResult result = camera->start();

            switch (result) {
                case START_RESULT_SUCCESS:
                    gui->logInfo("...camera started successfully!");
                    continue;
                case START_RESULT_ERROR:
                    gui->logError("Fatal error starting camera.");
                    break;
                case START_RESULT_NOT_FOUND:
                    gui->logError("No camera found. Retrying");
                    break;
            }
            // delay searching to avoid spamming the bus
            boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
        } else {
            // Camera is working, use it
            bool capture = triggered;
            if (capture) {
                triggerMutex.lock();
                triggered = false;
                triggerMutex.unlock();
            }

            if (capture) {
                // Do the capture
                gui->logDebug("Doing the capture");
                gui->hidePreviewImage();

                // We need to wait for the printer thread (not really hopefully)
                {
                    cout << "[Camera Thread] Waiting for printer thread to finish" << endl;
                    boost::unique_lock<boost::mutex> lk(printerStateMutex);
                    while (printerState != PRINTER_STATE_IDLE) {
                        printerStateCV.wait(lk);
                    }
                }


                // Update printer thread state to one
                {
                    boost::unique_lock<boost::mutex> lk(printerStateMutex);
                    printerState = PRINTER_STATE_WAITING_FOR_DATA;
                    printerStateCV.notify_all();
                }

                cancelPrintMutex.lock();
                printCanceled = false;
                cancelPrintMutex.unlock();

                // Get the mutex for the last jpeg image. We should have no problem doing this
                jpegImageMutex.lock();


                camera->triggerCaptureBlocking();

                triggerFlash();

                gui->logDebug("Successfully triggered");
                boost::this_thread::sleep(boost::posix_time::milliseconds(100));
                auto success = camera->readImageBlocking(&latestJpegBuffer, &latestJpegBufferSize, &latestJpegFileName,
                                                         &imageBuffer, &imageBufferSize, &imageInfo);
                jpegImageMutex.unlock();

                {
                    boost::unique_lock<boost::mutex> lk(printerStateMutex);
                    printerState = PRINTER_STATE_WAITING_FOR_USER_INPUT;
                    printerStateCV.notify_all();
                }

                if (success) {
                    gui->updatePreviewImage(imageBuffer, imageInfo.width, imageInfo.height);
                    gui->notifyFinalImageSent();
                    if (printerEnabled && button_serial_port.is_open())
                        button_serial_port.write_some(boost::asio::buffer("p", 1));

                    // 4500ms from here for the user to decide
                    boost::this_thread::sleep(boost::posix_time::milliseconds(4500));

                    // Notify the printer thread
                    {
                        boost::unique_lock<boost::mutex> lk(printerStateMutex);
                        printerState = PRINTER_STATE_WORKING;
                        printerStateCV.notify_all();
                    }
                } else {
                    gui->logError("Got an error");
                }

                if (button_serial_port.is_open())
                    button_serial_port.write_some(boost::asio::buffer("k", 1));

                gui->notifyPreviewIncoming();
            } else {
                // Get a preview
                auto success = camera->capturePreviewBlocking(&imageBuffer, &imageBufferSize, &imageInfo);
                if (success) {
                    gui->updatePreviewImage(imageBuffer, imageInfo.width, imageInfo.height);
                } else {
                    gui->logError("Error capturing preview. Restarting camera");
                    camera->stop();
                }
            }
        }
    }
    camera->stop();
}

void BoothLogic::logicThread() {
    gui->logDebug("Starting Logic Thread");

    while (isRunning) {
        // Send the heartbeat
        if (button_serial_port.is_open())
            button_serial_port.write_some(boost::asio::buffer(".", 1));
        //flash_serial_port.write_some(asio::buffer("i", 1));
        boost::this_thread::sleep(boost::posix_time::seconds(1));


        // check for an image and save it. but only if printer thread is idle
        bool allowSave = false;
        {
            boost::unique_lock<boost::mutex> lk(printerStateMutex);
            allowSave = (0 == printerState);
        }
        if (allowSave) {
            void *rawBuffer = nullptr;
            size_t rawSize = 0;
            std::string rawFilename;
            if (camera->getLastRawImage(&rawBuffer, &rawSize, &rawFilename)) {
                saveImage(rawBuffer, rawSize, rawFilename);
                free(rawBuffer);
            }
        }

        // check the printer state
        printerManager.refreshPrinterState();
        if (printerManager.getCurrentPrinterState() == STATE_STOPPED) {
            gui->addAlert("P", L"Drucker wurde gestoppt");
        } else {
            gui->removeAlert("P");
        }


        // check the camera state
        if (camera->getState() != STATE_WORKING) {
            gui->addAlert("C", L"Prüfe deine Kamera");
        } else {
            gui->removeAlert("C");
        }

        int freeStorage = getFreeStorageSpaceMB();
        if (freeStorage < 500) {
            if (freeStorage == -1)
                gui->addAlert("U", L"Kein Speichermedium gefunden");
            else
                gui->addAlert("U", L"Geringe Speicherkapazität: " + to_wstring(freeStorage) + L"MB");
        } else {
            gui->removeAlert("U");
        }

    }


}

void BoothLogic::ioThread() {
    cout << "IO Thread Started" << endl;
    while (isRunning) {
        if (button_serial_port.is_open()) {
            blocking_reader reader(button_serial_port, 500);
            while (reader.read_char(c) && c != '\n') {
                cout << "Got char: " << c << endl;
                switch (c) {
                    case 'a':
                        if (showAgreement) {
                            gui->hideAgreement();
                            showAgreement = false;
                            writeSettings();
                        }
                        break;
                    case 'c':
                        cancelPrintMutex.lock();
                        printCanceled = true;
                        cancelPrintMutex.unlock();
                        break;
                    case 't':
                        trigger();
                        break;
                    case 'd':
                        stop();
                        sync();
                        reboot(LINUX_REBOOT_CMD_POWER_OFF);
                    default:
                        break;
                }
            }
        }
    }
}

BoothLogic::~BoothLogic() {
    free(imageBuffer);
}

void BoothLogic::trigger() {
    if (showAgreement)
        return;

    triggerMutex.lock();
    triggered = true;
    triggerMutex.unlock();
}

void BoothLogic::printerThread() {
    while (isRunning) {
        bool do_print;
        {
            cout << "[Printer Thread] Waiting for an image to process" << endl;
            boost::unique_lock<boost::mutex> lk(printerStateMutex);
            while (printerState == PRINTER_STATE_IDLE || printerState == PRINTER_STATE_WAITING_FOR_DATA) {
                printerStateCV.timed_wait(lk, boost::posix_time::milliseconds(500));
                // application should close
                if (!isRunning)
                    return;
            }
            do_print = printerEnabled;
        }

        cout << "[Printer Thread] " << "Processing image. Printing enabled: " << do_print << endl;

        if (do_print) {
            // We need the final jpeg image. So lock the mutex
            {
                boost::unique_lock<boost::mutex> lk(jpegImageMutex);

                // first we save the image
                saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName);

                Magick::Image toPrepare;
                if(templateEnabled) {
                    toPrepare = imageProcessor.frameImageForPrint(latestJpegBuffer, latestJpegBufferSize);
                } else {
                    toPrepare = imageProcessor.decodeImageForPrint(latestJpegBuffer, latestJpegBufferSize);
                }
                cout << "[Printer Thread] " << "Framed" << endl;
                printerManager.prepareImageForPrint(toPrepare);
                cout << "[Printer Thread] " << "Prepared" << endl;
            }

            {
                cout << "[Printer Thread] Waiting for the user to decide if he wants to print" << endl;
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                while (printerState == PRINTER_STATE_WAITING_FOR_USER_INPUT) {
                    printerStateCV.wait(lk);
                }
            }


            // We need the info if the user wants to print or not
            {
                boost::unique_lock<boost::mutex> lk(cancelPrintMutex);
                if (!printCanceled) {
                    cout << "[Printer Thread] " << "Printing" << endl;
                    printerManager.printImage();
                } else {
                    cout << "[Printer Thread] " << "Printing canceled" << endl;
                    printerManager.cancelPrint();
                }
            }
        } else {
            // We need the final jpeg image. So lock the mutex
            {
                boost::unique_lock<boost::mutex> lk(jpegImageMutex);

                // we only need to save the image
                saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName);
            }

            // wait for logic thread
            {
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                while (printerState == PRINTER_STATE_WAITING_FOR_USER_INPUT) {
                    printerStateCV.wait(lk);
                }
            }
        }

        {
            boost::unique_lock<boost::mutex> lk(printerStateMutex);
            printerState = PRINTER_STATE_IDLE;
            printerStateCV.notify_all();
        }
    }
}

int BoothLogic::getFreeStorageSpaceMB() {
    if (imageDir.empty()) {
        cerr << "No image dir specified" << endl;
        return -1;
    }

    boost::filesystem::space_info s = boost::filesystem::space(imageDir);

    return static_cast<int>(s.free / 1024 / 1024);
}

void BoothLogic::saveImage(void *data, size_t size, std::string filename) {
    if (imageDir.empty()) {
        cerr << "No image dir specified" << endl;
        return;
    }

    std::time_t time = std::time(nullptr);

    std::string fullImagePath = imageDir;

    fullImagePath += "/";
    fullImagePath += to_string((long) time);
    fullImagePath += "_";
    fullImagePath += filename;


    cout << "Writing image to:" << fullImagePath << endl;

    FILE *fp;

    fp = fopen(fullImagePath.c_str(), "wb");
    if (fp == nullptr) {
        cerr << "Error opening output file" << endl;
        return;
    }

    fwrite(data, size, 1, fp);

    fclose(fp);

    cout << "File written to: " << fullImagePath << endl;
}

void BoothLogic::stopForUpdate() {
    if (button_serial_port.is_open())
        button_serial_port.write_some(boost::asio::buffer("f", 1));

    returnCode = 0x42;
    stop();
}

void BoothLogic::setPrinterEnabled(bool printerEnabled, bool persist) {
    this->printerEnabled= printerEnabled;
    gui->setPrinterEnabled(printerEnabled);
    if(persist) {
        writeSettings();
    }
}

bool BoothLogic::getPrinterEnabled() {
    return printerEnabled;
}

void BoothLogic::readSettings() {
    boost::property_tree::ptree ptree;

    bool success = true;
    try {
        boost::property_tree::read_json(std::string(getenv("HOME")) + "/.selfomat_settings.json", ptree);
    } catch (boost::exception &e) {
        cerr << "Error loading settings settings. Writing defaults. Error was: " << boost::diagnostic_information(e) << endl;
        success = false;
    }

    setPrinterEnabled(ptree.get<bool>("printer_enabled", true));
    setTemplateEnabled(ptree.get<bool>("template_enabled", false));
    this->showAgreement=ptree.get<bool>("show_agreement", true);
    this->flashEnabled=ptree.get<bool>("flash_enabled", true);
    this->flashDurationMicros=ptree.get<uint64_t>("flash_duration_micros", 100000);
    this->flashDelayMicros=ptree.get<uint64_t>("flash_delay_micros", 0);
    this->flashBrightness=ptree.get<float>("flash_brightness", 1.0f);
    this->flashFade=ptree.get<float>("flash_fade", 0.0f);

    if(!success)
        writeSettings();
}

void BoothLogic::writeSettings() {
    boost::property_tree::ptree ptree;
    ptree.put("show_agreement", showAgreement);
    ptree.put("printer_enabled", printerEnabled);
    ptree.put("template_enabled", templateEnabled);
    ptree.put("flash_enabled", this->flashEnabled);
    ptree.put("flash_duration_micros", this->flashDurationMicros);
    ptree.put("flash_delay_micros", this->flashDelayMicros);
    ptree.put("flash_brightness", this->flashBrightness);
    ptree.put("flash_fade", this->flashFade);

    try {
        boost::property_tree::write_json(std::string(getenv("HOME")) + "/.selfomat_settings.json", ptree);
    } catch (boost::exception &e) {
        cerr << "Error writing settings. Error was: " << boost::diagnostic_information(e) << endl;
    }
}

void BoothLogic::setFlashParameters(bool enabled, float brightness, float fade, uint64_t delayMicros,
                                    uint64_t durationMicros, bool persist) {
    this->flashEnabled = enabled;
    this->flashBrightness = brightness;
    this->flashFade = fade;
    this->flashDelayMicros = delayMicros;
    this->flashDurationMicros = durationMicros;
    if(persist) {
        writeSettings();
    }
}

void BoothLogic::getFlashParameters(bool *enabled, float *brightness, float *fade, uint64_t *delayMicros,
                                    uint64_t *durationMicros) {
     *enabled = this->flashEnabled;
     *brightness = this->flashBrightness;
     *fade = this->flashFade;
     *delayMicros = this->flashDelayMicros;
     *durationMicros = this->flashDurationMicros;
}

void BoothLogic::setTemplateEnabled(bool templateEnabled, bool persist) {
    this->templateEnabled = templateEnabled;
    gui->setTemplateEnabled(templateEnabled);
    if(persist)
        writeSettings();
}

bool BoothLogic::getTemplateEnabled() {
    return templateEnabled;
}
