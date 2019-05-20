//
// Created by clemens on 21.01.19.
//

#include "BoothLogic.h"

using namespace std;
using namespace selfomat::logic;
using namespace selfomat::camera;
using namespace selfomat::ui;

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
            sendCommand('!');
        else
            sendCommand('?');
        sendCommand('.');

        setLEDOffset(ledOffset);
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
        sendCommand('a');
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

    sendCommand('#');
}

void BoothLogic::stop() {
    std::cout << "stopping logic" << std::endl;
    isRunning = false;

    if (logicThreadHandle.joinable()) {
        cout << "waiting for logic" << endl;
        logicThreadHandle.join();
    }

    writeSettings();

    if (button_serial_port.is_open())
        button_serial_port.close();

    if (ioThreadHandle.joinable()) {
        cout << "waiting for io" << endl;
        ioThreadHandle.join();
    }
    if (cameraThreadHandle.joinable()) {
        cout << "waiting for cam" << endl;
        cameraThreadHandle.join();
    }
    if (printThreadHandle.joinable()) {
        cout << "waiting for print" << endl;
        printThreadHandle.join();
    }

    if (gui != nullptr) {
        gui->stop();
        gui = NULL;
    }

    imageProcessor.stop();
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
                        sendCommand('p');

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

                sendCommand('k');

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
        sendCommand('.');
        //flash_serial_port.write_some(asio::buffer("i", 1));
        boost::this_thread::sleep(boost::posix_time::seconds(1));


        // check for an image and save it. but only if printer thread is idle
        // THIS IS ONLY FOR RAW FILES!!!
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
                saveImage(rawBuffer, rawSize, rawFilename, true);
                free(rawBuffer);
            }
        }

        // check the printer state
        printerManager.refreshPrinterState();
        if (printerManager.getCurrentPrinterState() == STATE_STOPPED && printerEnabled) {
            gui->addAlert(ALERT_PRINTER, L"Drucker wurde gestoppt");
        } else {
            gui->removeAlert(ALERT_PRINTER);
        }


        // check the camera state
        if (camera->getState() != STATE_WORKING) {
            gui->addAlert(ALERT_CAMERA, L"Prüfe deine Kamera");
        } else {
            gui->removeAlert(ALERT_CAMERA);
        }

        int freeStorage = getFreeStorageSpaceMB();
        if (freeStorage < 500) {
            if (freeStorage == -1)
                gui->addAlert(ALERT_STORAGE, L"Kein Speichermedium gefunden");
            else
                gui->addAlert(ALERT_STORAGE, L"Geringe Speicherkapazität: " + to_wstring(freeStorage) + L"MB");
        } else {
            gui->removeAlert(ALERT_STORAGE);
        }

    }

    // Sync to disk
    cout << "Syncing changes to disk" << endl;
    boost::thread syncThreadHandle(sync);
    while(!syncThreadHandle.try_join_for(boost::chrono::milliseconds(1000))) {
        cout << "Still syncing..." << endl;
        sendCommand('.');
    }
    cout << "Syncing done" << endl;
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
                        returnCode = -1;
                        isRunning = false;
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

void BoothLogic::enableStressTest() {
    sendCommand('S');
}

void BoothLogic::disableStressTest() {
    sendCommand('s');
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
                saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName, true);

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
                saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName, true);
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

bool BoothLogic::isMountpoint(std::string folder) {
    /* get the parent directory  of the file */

    std::string folder_cpy(folder.c_str());
    std::string parent_name = dirname((char*)folder_cpy.c_str());

    /* get the file's stat info */
    struct stat file_stat{};
    if( -1 == stat(folder.c_str(), &file_stat) ) {
        cerr << "stat error" << endl;
        return false;
    }

    /* determine whether the supplied file is a directory
      if it isn't, then it can't be a mountpoint. */
    if( !(file_stat.st_mode & S_IFDIR) ) {
        cerr << "image dir is not a directory" << endl;
        return false;
    }

    /* get the parent's stat info */
    struct stat parent_stat{};
    if( -1 == stat(parent_name.c_str(), &parent_stat) ) {
        cout << "parent stat fail" << endl;
        return false;
    }

    // it's a mount point if devices differ
    return file_stat.st_dev != parent_stat.st_dev;
}

bool BoothLogic::saveImage(void *data, size_t size, std::string filename, bool showAlert) {
    auto success = saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName);
    if(!success && showAlert) {
        gui->addAlert(ALERT_STORAGE_ERROR, L"Fehler beim Speichern des Fotos", true);
    }

    return success;
}

bool BoothLogic::saveImage(void *data, size_t size, std::string filename) {
    if (!storageEnabled) {
        return true;
    }

    if (imageDir.empty()) {
        cerr << "No image dir specified" << endl;
        return false;
    }

    if(!isMountpoint(imageDir)) {
        cerr << "imageDir not a mountpoint" << endl;
        return false;
    }

    // Check if imageDir is a mountpoint

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
        return false;
    }

    fwrite(data, size, 1, fp);

    fclose(fp);

    cout << "File written to: " << fullImagePath << endl;

    return true;
}

void BoothLogic::stopForUpdate() {
    sendCommand('f');

    returnCode = 0x42;
    stop();
}

void BoothLogic::setStorageEnabled(bool storageEnabled, bool persist) {
    this->storageEnabled = storageEnabled;
    if(persist) {
        writeSettings();
    }
}

bool BoothLogic::getStorageEnabled() {
    return storageEnabled;
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

    setStorageEnabled(ptree.get<bool>("storage_enabled", true));
    setPrinterEnabled(ptree.get<bool>("printer_enabled", true));
    setTemplateEnabled(ptree.get<bool>("template_enabled", false));
    this->showAgreement=false;
    this->flashEnabled=ptree.get<bool>("flash_enabled", true);
    this->flashDurationMicros=ptree.get<uint64_t>("flash_duration_micros", 100000);
    this->flashDelayMicros=ptree.get<uint64_t>("flash_delay_micros", 0);
    this->flashBrightness=ptree.get<float>("flash_brightness", 1.0f);
    this->flashFade=ptree.get<float>("flash_fade", 0.0f);
    setLEDOffset(ptree.get<int8_t>("led_offset", 0));

    if(!success)
        writeSettings();
}

void BoothLogic::writeSettings() {
    boost::property_tree::ptree ptree;
    ptree.put("show_agreement", showAgreement);
    ptree.put("storage_enabled", storageEnabled);
    ptree.put("printer_enabled", printerEnabled);
    ptree.put("template_enabled", templateEnabled);
    ptree.put("flash_enabled", this->flashEnabled);
    ptree.put("flash_duration_micros", this->flashDurationMicros);
    ptree.put("flash_delay_micros", this->flashDelayMicros);
    ptree.put("flash_brightness", this->flashBrightness);
    ptree.put("flash_fade", this->flashFade);
    ptree.put("led_offset", this->ledOffset);

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

void BoothLogic::flashTest() {
    auto duration = static_cast<uint8_t>(this->flashDurationMicros);
    sendCommand('$', duration);
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

bool BoothLogic::getTemplateLoaded() {
    return imageProcessor.isTemplateLoaded();
}



void BoothLogic::setLEDOffset(int8_t offset, bool persist) {
    this->ledOffset = offset;
    
    if(persist) {
        sendCommand('L', offset+8);
        writeSettings();
    } else {
        sendCommand('l', offset+8);
    }
}

int8_t BoothLogic::getLEDOffset() {
    return ledOffset;
}

void BoothLogic::sendCommand(uint8_t command, uint8_t argument) {
    if (!button_serial_port.is_open())
        return;

    std::vector<uint8_t> data;
    data.push_back(command);
    data.push_back(argument);

    button_serial_port.write_some(boost::asio::buffer(data.data(), 2));
}

void BoothLogic::sendCommand(uint8_t command) {
    if (!button_serial_port.is_open())
        return;
    button_serial_port.write_some(boost::asio::buffer(&command, 1));
}
