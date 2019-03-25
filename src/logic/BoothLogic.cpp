//
// Created by clemens on 21.01.19.
//

#include "BoothLogic.h"

using namespace std;
using namespace boost;
using namespace selfomat::logic;
using namespace selfomat::camera;

/**
 * Looks for arduinos in /dev and returns all found ones
 * @return
 */
vector<boost::filesystem::path> BoothLogic::findArduinos() {
    filesystem::path devPath("/dev/pts");

    vector<filesystem::path> foundArduinos;
    for (auto &e : make_iterator_range(filesystem::directory_iterator(devPath))) {
        if (starts_with(e.path().string(), button_port))
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
        button_serial_port.set_option(asio::serial_port_base::baud_rate(9600));
        button_serial_port.write_some(asio::buffer("!", 1));
        button_serial_port.write_some(asio::buffer(".", 1));
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
        tmp_serial_port.set_option(asio::serial_port_base::baud_rate(9600));
        tmp_serial_port.write_some(asio::buffer("i", 1));

        cout << "Waiting for identification" << endl;
        char c;
        auto count = asio::read(tmp_serial_port, asio::buffer(&c, 1));
        tmp_serial_port.close();
        if(count > 0) {
            cout << "Got a " << c << endl;
            if (c == 'b') {
                cout << "Found the button" << endl;
                return connectButton(serialPath);
            }
            cout << "Unknown identification: " << c << endl;
        } else {
            cout << "No identification received" << endl;
        }
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

    gui->logDebug("Initializing Image Processor");
    if(!imageProcessor.start())
        return false;

    gui->logDebug("Starting Printer");
    if(!printerManager.start())
        return false;

    if(has_button) {
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




    CameraStartResult result;

    do {
        gui->logDebug("Starting Camera");
        result = camera->start();
        switch (result) {
            case START_RESULT_SUCCESS:
                isRunning = true;
                gui->logInfo("...camera started successfully!");
                // Start the logic thread
                ioThreadHandle = boost::thread(boost::bind(&BoothLogic::ioThread, this));
                logicThreadHandle = boost::thread(boost::bind(&BoothLogic::logicThread, this));
                cameraThreadHandle = boost::thread(boost::bind(&BoothLogic::cameraThread, this));
                printThreadHandle = boost::thread(boost::bind(&BoothLogic::printerThread, this));
                return true;
            case START_RESULT_ERROR:
                gui->logError("Fatal error starting camera.");
                return false;
            case START_RESULT_NOT_FOUND:
                gui->logError("No camera found. Retrying");
                break;

        }

        boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
    } while (true);
}

void BoothLogic::triggerFlash() {
    #ifdef USE_SPI
        digitalWrite(PIN_SS, LOW);
        flash_struct flash;
        flash.delay = 0;
        flash.duration = 1000;
        flash.brightness=255;
        flash.fade = 255;
        wiringPiSPIDataRW(0, (unsigned char*)&flash, 6);
        digitalWrite(PIN_SS, HIGH);
    #endif
}

void BoothLogic::stop() {
    std::cout << "stopping logic" << std::endl;

    camera->stop();
    if(button_serial_port.is_open())
        button_serial_port.close();

    gui->stop();
    isRunning = false;
    imageProcessor.stop();

    logicThreadHandle.join();
    ioThreadHandle.join();
    cameraThreadHandle.join();
    printThreadHandle.join();
}

void BoothLogic::cameraThread() {
    gui->logDebug("Starting Camera Thread");
    gui->initialized();
    while(isRunning) {

        bool capture = triggered;
        if(capture) {
            triggerMutex.lock();
            triggered = false;
            triggerMutex.unlock();
        }

        if(capture) {
            // Do the capture
            gui->logDebug("Doing the capture");
            gui->hidePreviewImage();

            // We need to wait for the printer thread (not really hopefully)
            {
                cout << "[Camera Thread] Waiting for printer thread to finish" << endl;
                unique_lock<boost::mutex> lk(printerStateMutex);
                while (printerState != PRINTER_STATE_IDLE) {
                    printerStateCV.wait(lk);
                }
            }


            // Update printer thread state to one
            {
                unique_lock<boost::mutex> lk(printerStateMutex);
                printerState = PRINTER_STATE_WAITING_FOR_DATA;
                printerStateCV.notify_all();
            }

            cancelPrintMutex.lock();
            printCanceled = false;
            cancelPrintMutex.unlock();

            // Get the mutex for the last jpeg image. We should have no problem doing this
            jpegImageMutex.lock();


            auto triggerSuccess = camera->triggerCaptureBlocking();

            triggerFlash();

            gui->logDebug("Successfully triggered");
            boost::this_thread::sleep(boost::posix_time::milliseconds(100));
            auto success = camera->readImageBlocking(&latestJpegBuffer, &latestJpegBufferSize, &latestJpegFileName,
                    &imageBuffer, &imageBufferSize, &imageInfo);
            jpegImageMutex.unlock();

            {
                unique_lock<boost::mutex> lk(printerStateMutex);
                printerState = PRINTER_STATE_WAITING_FOR_USER_INPUT;
                printerStateCV.notify_all();
            }

            if (success) {
                gui->updatePreviewImage(imageBuffer, imageInfo.width, imageInfo.height);
                gui->notifyFinalImageSent();

                // 4500ms from here for the user to decide
                boost::this_thread::sleep(boost::posix_time::milliseconds(4500));

                // Notify the printer thread
                {
                    unique_lock<boost::mutex> lk(printerStateMutex);
                    printerState = PRINTER_STATE_WORKING;
                    printerStateCV.notify_all();
                }
            } else {
                gui->logError("Got an error");
            }

            if(button_serial_port.is_open())
                button_serial_port.write_some(asio::buffer("k", 1));

            gui->notifyPreviewIncoming();
        } else {
            // Get a preview
            auto success = camera->capturePreviewBlocking(&imageBuffer, &imageBufferSize, &imageInfo);
            if(success) {
                gui->updatePreviewImage(imageBuffer, imageInfo.width, imageInfo.height);
            } else {
                gui->logError("Error capturing preview.");
            }
        }
    }
}

void BoothLogic::logicThread() {
    gui->logDebug("Starting Logic Thread");

    while (isRunning) {
        // Send the heartbeat
        if(button_serial_port.is_open())
            button_serial_port.write_some(asio::buffer(".", 1));
        //flash_serial_port.write_some(asio::buffer("i", 1));
        boost::this_thread::sleep(boost::posix_time::seconds(1));


        // check for an image and save it. but only if printer thread is idle
        bool allowSave = false;
        {
            unique_lock<boost::mutex> lk(printerStateMutex);
            allowSave = (0 == printerState);
        }
        if(allowSave) {
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
    while(isRunning) {
        if(button_serial_port.is_open()) {
            auto count = asio::read(button_serial_port, asio::buffer(&c, 1));
            if (count > 0) {
                cout << "Got char: " << c << endl;
                switch (c) {
                    case 'c':
                        cancelPrintMutex.lock();
                        printCanceled = true;
                        cancelPrintMutex.unlock();
                        break;
                    case 't':
                        triggerMutex.lock();
                        triggered = true;
                        triggerMutex.unlock();
                        break;
                    case 'd':
                        stop();
                    default:break;
                }
            }
        }
    }
}

BoothLogic::~BoothLogic() {
    free(imageBuffer);
}

void BoothLogic::trigger() {
    triggerMutex.lock();
    triggered = true;
    triggerMutex.unlock();
}

void BoothLogic::printerThread() {
    while(isRunning) {
        {
            cout << "[Printer Thread] Waiting for an image to process" << endl;
            unique_lock<boost::mutex> lk(printerStateMutex);
            while (printerState == PRINTER_STATE_IDLE || printerState == PRINTER_STATE_WAITING_FOR_DATA) {
                printerStateCV.wait(lk);
            }
        }

        cout << "[Printer Thread] " << "Processing image" << endl;

        // We need the final jpeg image. So lock the mutex
        {
            unique_lock<boost::mutex> lk(jpegImageMutex);

            // first we save the image
            saveImage(latestJpegBuffer, latestJpegBufferSize, latestJpegFileName);

            Magick::Image framed = imageProcessor.frameImageForPrint(latestJpegBuffer, latestJpegBufferSize);
            cout << "[Printer Thread] " << "Framed" << endl;
            printerManager.prepareImageForPrint(framed);
            cout << "[Printer Thread] " << "Prepared" << endl;
        }

        {
            cout << "[Printer Thread] Waiting for the user to decide if he wants to print" << endl;
            unique_lock<boost::mutex> lk(printerStateMutex);
            while (printerState == PRINTER_STATE_WAITING_FOR_USER_INPUT) {
                printerStateCV.wait(lk);
            }
        }


        // We need the info if the user wants to print or not
        {
            unique_lock<boost::mutex> lk(cancelPrintMutex);
            if (!printCanceled) {
                cout << "[Printer Thread] " << "Printing" << endl;
                printerManager.printImage();
            } else {
                cout << "[Printer Thread] " << "Printing canceled" << endl;
                printerManager.cancelPrint();
            }
        }


        cout << "[Printer Thread] I'm done with my work, so i'll just update the printer state to IDLE" << endl;
        {
            unique_lock<boost::mutex> lk(printerStateMutex);
            printerState = PRINTER_STATE_IDLE;
            printerStateCV.notify_all();
        }
    }
}

int BoothLogic::getFreeStorageSpaceMB() {
    if(imageDir.empty()) {
        cerr << "No image dir specified" << endl;
        return -1;
    }

    filesystem::space_info s = filesystem::space(imageDir);

    return (s.free / 1024 / 1024);
}

void BoothLogic::saveImage(void *data, size_t size, std::string filename) {
    if(imageDir.empty()) {
        cerr << "No image dir specified" << endl;
        return;
    }

    std::time_t time = std::time(nullptr);

    std::string fullImagePath = imageDir;

    fullImagePath+="/";
    fullImagePath+=to_string((long)time);
    fullImagePath+="_";
    fullImagePath+=filename;


    cout << "Writing image to:" << fullImagePath << endl;

    FILE *fp;

    fp = fopen(fullImagePath.c_str(), "wb");
    if(fp == nullptr) {
        cerr << "Error opening output file" << endl;
        return;
    }

    fwrite(data, size, 1, fp);

    fclose(fp);

    cout << "File written to: " << fullImagePath << endl;
}