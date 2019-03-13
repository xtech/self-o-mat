//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_BOOTHLOGIC_H
#define SELF_O_MAT_BOOTHLOGIC_H


#include "ICamera.h"
#include "ImageProcessor.h"
#include "PrinterManager.h"
#include <iostream>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/asio/streambuf.hpp>
#include <boost/asio.hpp>
#include <boost/lexical_cast.hpp>
#include <Magick++.h>

#include <boost/algorithm/hex.hpp>

//#define USE_SPI

#ifdef USE_SPI
    #include <wiringPi.h>
    #include <wiringPiSPI.h>

    #define PIN_SS 25
#endif

// 2 bytes delay in 100 μs, 2 bytes duration in 100 μs, 1 byte brightness, 1 byte fading in μs
struct flash_struct {
    uint16_t delay;
    uint16_t duration;
    uint8_t brightness;
    uint8_t fade;
}  __attribute__((packed));

class BoothLogic {
public:
    explicit BoothLogic(ICamera *camera, IGui *gui, bool has_button, const string &button_port, bool has_flash, string printer_name) : camera(camera), gui(gui), io_service(), tmp_serial_port(io_service), button_serial_port(io_service),
        imageProcessor(gui), printerManager(gui, printer_name),has_button(has_button), button_port(button_port), has_flash(has_flash) {
        triggered = false;

        // TODO: Load from json file
        printer_enabled = true;
    }


private:
    bool has_button, has_flash;
    string button_port;

    bool printer_enabled;

    PrinterManager printerManager;
    ImageProcessor imageProcessor;

    bool isRunning;
    boost::mutex triggerMutex;
    bool triggered;

    // We have a second thread running which first tries to get the jpegImageMutex
    // as soon as it has the jpegImageMutex it prepares for printing.
    boost::mutex jpegImageMutex;
    // Then the thread waits for the cancelPrintMutex. As soon as it has the cancelPrintMutex it checks if it
    // really needs to print or not. if it wants to print it prints.
    // Finally it unlocks both mutex and exits
    boost::mutex cancelPrintMutex;
    bool printCanceled = false;


    boost::asio::io_service io_service;
    boost::asio::serial_port button_serial_port;
    boost::asio::serial_port tmp_serial_port;
    char c{};

    ICamera *camera;
    IGui *gui;

    /* The image buffer for the result image */
    void* imageBuffer = nullptr;
    size_t imageBufferSize = 0;
    ImageInfo imageInfo{};


    void *latestJpegBuffer = nullptr;
    size_t latestJpegBufferSize = 0;

    boost::condition_variable printerStateCV;
    boost::mutex printerStateMutex;
    int printerState = 0;

    boost::thread logicThreadHandle;
    boost::thread ioThreadHandle;
    boost::thread cameraThreadHandle;
    boost::thread printThreadHandle;

    bool connectButton(boost::filesystem::path serialPath);
    bool connectToSerial(boost::filesystem::path serialPath);
    void cameraThread();
    void logicThread();
    void ioThread();
    void printerThread();

    void triggerFlash();


    vector<boost::filesystem::path> findArduinos();



public:
    void trigger();
    bool start();
    void stop();
    void join() {
        logicThreadHandle.join();
        ioThreadHandle.join();
        cameraThreadHandle.join();
    }

    virtual ~BoothLogic();

};


#endif //SELF_O_MAT_BOOTHLOGIC_H
