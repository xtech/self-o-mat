//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_PRINTERMANAGER_H
#define SELF_O_MAT_PRINTERMANAGER_H

#include <cups/cups.h>
#include <iostream>
#include "ILogger.h"
#include <Magick++.h>
#include "buffers.h"

class PrinterManager {
private:
    ILogger *logger;
    std::string printer_name;

    void *imageTmpBuffer = nullptr;
    size_t imageTmpBufferSize = 0;

    bool hasImagePrepared = false;
    size_t sizeOfPreparedImage = 0;

public:
    PrinterManager(ILogger *logger, std::string printer_name);

    //std::vector<std::string> PrinterManager::listPrinters();

    bool prepareImageForPrint(Magick::Image image);

    bool start();

    bool printImage();

    bool cancelPrint();
};


#endif //SELF_O_MAT_PRINTERMANAGER_H
