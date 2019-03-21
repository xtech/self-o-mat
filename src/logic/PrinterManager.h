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

#include <boost/thread.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace selfomat::tools;

namespace selfomat {
    namespace logic {
        class PrinterManager {
        private:
            enum PRINTER_STATE {
                STATE_UNKNOWN,
                STATE_IDLE,
                STATE_PRINTING,
                STATE_STOPPED
            };

            PRINTER_STATE currentPrinterState_;
            std::vector<std::string> currentStateReasons_;

            ILogger *logger;
            std::string printer_name;

            void *imageTmpBuffer = nullptr;
            size_t imageTmpBufferSize = 0;

            bool hasImagePrepared = false;
            size_t sizeOfPreparedImage = 0;

            cups_dest_t *cupsDestinations = nullptr;
            int cupsDestinationCount = 0;

            boost::mutex printerStateMutex;

        public:
            const int currentPrinterState() {
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                return currentPrinterState_;
            }
            const std::vector<std::string>& currentStateReasons() {
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                return currentStateReasons_;
            }

            bool refreshCupsDestinations();

            bool refreshPrinterState();

            PrinterManager(ILogger *logger, std::string printer_name);

            bool prepareImageForPrint(Magick::Image image);

            bool start();

            bool resumePrinter();

            bool printImage();

            bool cancelPrint();
        };
    }
}

#endif //SELF_O_MAT_PRINTERMANAGER_H
