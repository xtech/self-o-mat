//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_PRINTERMANAGER_H
#define SELF_O_MAT_PRINTERMANAGER_H


#include <cups/cups.h>
#include <cups/ipp.h>
#include <iostream>
#include "tools/ILogger.h"
#include <Magick++.h>
#include "../tools/buffers.h"

using namespace selfomat::tools;

namespace selfomat {
    namespace logic {
        class PrinterManager {
        private:
            ILogger *logger;
            std::string printer_name;

            void *imageTmpBuffer = nullptr;
            size_t imageTmpBufferSize = 0;

            bool hasImagePrepared = false;
            size_t sizeOfPreparedImage = 0;

            cups_dest_t *cupsDestinations = nullptr;
            int cupsDestinationCount = 0;


        public:
            bool refreshCupsDestinations();

            PrinterManager(ILogger *logger, std::string printer_name);

            bool prepareImageForPrint(Magick::Image image);

            bool start();

            bool printImage();

            bool cancelPrint();
        };
    }
}

#endif //SELF_O_MAT_PRINTERMANAGER_H
