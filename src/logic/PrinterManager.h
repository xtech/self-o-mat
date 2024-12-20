//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_PRINTERMANAGER_H
#define SELF_O_MAT_PRINTERMANAGER_H


#include <cups/cups.h>
#include <cups/ipp.h>
#include <cups/adminutil.h>
#include <iostream>
#include <opencv2/opencv.hpp>
#include "../tools/buffers.h"

#include <boost/thread.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

using namespace selfomat::tools;

namespace selfomat {
    namespace logic {

        enum PRINTER_STATE {
            STATE_UNKNOWN,
            STATE_IDLE,
            STATE_PRINTING,
            STATE_STOPPED
        };

        enum PrinterJobState {
            JOB_STATE_UNKNOWN,
            JOB_STATE_PENDING,
            JOB_STATE_HELD,
            JOB_STATE_PROCESSING,
            JOB_STATE_STOPPED,
            JOB_STATE_CANCELED,
            JOB_STATE_ABORTED,
            JOB_STATE_COMPLETED
        };

        enum PrinterAttentionFlag {
            PRINTER_ATTN_NO_PAPER = (1 << 0),
            PRINTER_ATTN_NO_INK = (1 << 1),
            PRINTER_ATTN_NO_TRAY = (1 << 2)
        };

        class PrinterManager {
        private:
            static std::string TAG;
            PRINTER_STATE currentPrinterState;
            std::vector<std::string> currentStateReasons;

            std::string printer_name;

            void *imageTmpBuffer = nullptr;
            size_t imageTmpBufferSize = 0;

            bool hasImagePrepared = false;
            size_t sizeOfPreparedImage = 0;

            cups_dest_t *cupsDestinations = nullptr;
            int cupsDestinationCount = 0;

            boost::mutex printerStateMutex;

        public:
            const PRINTER_STATE getCurrentPrinterState() {
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                return currentPrinterState;
            }
            const std::vector<std::string>& getCurrentStateReasons() {
                boost::unique_lock<boost::mutex> lk(printerStateMutex);
                return currentStateReasons;
            }

            bool refreshCupsDevices();

            bool refreshCupsDestinations();

            bool refreshPrinterState();

            PrinterManager();

            bool prepareImageForPrint(cv::Mat image);

            bool start();

            bool resumePrinter();

            int printImage();

            bool cancelPrint();

            bool getJobDetails(int jobId, PrinterJobState &state, time_t &creationTs, time_t &processingTs, time_t &completedTs);

            static const char* printerJobStateToString(PrinterJobState &state);

            void checkPrinterAttentionFromJob(int jobId, unsigned int &flags);
        };
    }
}

#endif //SELF_O_MAT_PRINTERMANAGER_H
