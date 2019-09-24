#include <utility>

//
// Created by clemens on 12.02.19.
//

#include "PrinterManager.h"

using namespace selfomat::logic;

std::string PrinterManager::TAG = "PRINTER_MANAGER";

bool PrinterManager::start() {
    //refreshCupsDevices();
    refreshPrinterState();
    resumePrinter();
    return true;
}

bool PrinterManager::refreshCupsDevices() {

    cupsGetDevices(CUPS_HTTP_DEFAULT, 5, "usb", CUPS_EXCLUDE_NONE,
            (cups_device_cb_t) [] (const char *device_class,
                                   const char *device_id, const char *device_info,
                                   const char *device_make_and_model,
                                   const char *device_uri,
                                   const char *device_location, void *user_data) {

                LOG_D(TAG, "Found cups devices: ", device_uri);

        }, this);

    return true;
}

bool PrinterManager::refreshCupsDestinations() {

    cupsFreeDests(cupsDestinationCount, cupsDestinations);
    cupsDestinationCount = 0;

    // We do NOT want to discover the network, mask = (CUPS_PRINTER_DISCOVERED | CUPS_PRINTER_REMOTE)
    cupsEnumDests(CUPS_DEST_FLAGS_NONE, 300, NULL, CUPS_PRINTER_LOCAL, (CUPS_PRINTER_DISCOVERED | CUPS_PRINTER_REMOTE),
            (cups_dest_cb_t) [] (void *user_data, unsigned flags, cups_dest_t *dest) {
                auto *printerManager = (PrinterManager*)user_data;
                auto printerName = std::string (dest->name);

                if (dest->is_default && printerManager->printer_name != printerName) {
                    LOG_D(TAG, "Found default printer:", printerName);
                    printerManager->printer_name = printerName;
                }

                printerManager->cupsDestinationCount =
                        cupsCopyDest(dest, printerManager->cupsDestinationCount,
                                     &printerManager->cupsDestinations);

                return 1;
        }, this);

    if (!printer_name.empty()) {
        bool foundDefault = false;
        for(int i = 0; i < cupsDestinationCount; i++) {
            if (std::string (cupsDestinations[i].name) == printer_name) {
                foundDefault = true;
                break;
            }
        }
        if (!foundDefault) {
            LOG_D(TAG, "Removed old default printer.");
            printer_name.clear();
        }
    }

    if (printer_name.empty() && cupsDestinationCount > 0) {
        LOG_D(TAG, "Found any printer:", cupsDestinations[0].name);
        printer_name = std::string (cupsDestinations[0].name);
    }

    return true;
}

bool PrinterManager::refreshPrinterState() {

    boost::unique_lock<boost::mutex> lk(printerStateMutex);

    cups_dest_t     *dest = NULL;
    const char      *printer_state_reasons = NULL;
    int             printer_state = -1;

    PRINTER_STATE oldPrinterState = currentPrinterState;


    // Unfortunately necessary in order to get the current state
    refreshCupsDestinations();

    currentStateReasons.clear();

    if (printer_name.empty()) {
        currentPrinterState = STATE_UNKNOWN;
        return false;
    }

    dest = cupsGetDest(printer_name.c_str(),
                       NULL,
                       cupsDestinationCount,
                       cupsDestinations);
    if (dest) {
        printer_state = atoi(cupsGetOption("printer-state",
                                           dest->num_options,
                                           dest->options));

        printer_state_reasons = cupsGetOption("printer-state-reasons",
                                              dest->num_options,
                                              dest->options);
    }

    switch(printer_state) {
        case 3 :
            currentPrinterState = STATE_IDLE;
            break;
        case 4 :
            currentPrinterState = STATE_PRINTING;
            break;
        case 5 :
            currentPrinterState = STATE_STOPPED;
            break;
        default:
            currentPrinterState = STATE_UNKNOWN;
    }

    if(printer_state_reasons == nullptr)
        return false;

    boost::split(currentStateReasons,
                 printer_state_reasons,
                 boost::is_any_of(", "),
                 boost::token_compress_on);


    if (oldPrinterState != currentPrinterState) {
        LOG_D(TAG, "Current printer state: ", std::to_string(currentPrinterState));
    }

    return true;
}

bool PrinterManager::resumePrinter() {

    if (printer_name.empty()) {
        return false;
    }

    boost::unique_lock<boost::mutex> lk(printerStateMutex);

    http_t      *http;
    ipp_t		*request;
    char		uri[HTTP_MAX_URI];

    http = httpConnect2(cupsServer(),
                        ippPort(),
                        NULL,
                        AF_UNSPEC,
                        cupsEncryption(),
                        1,
                        30000,
                        NULL);


    httpAssembleURIf(HTTP_URI_CODING_ALL, uri, sizeof(uri), "ipp", NULL, "localhost", ippPort(), "/printers/%s", printer_name.c_str());

    request = ippNewRequest(IPP_OP_ENABLE_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    ippDelete(cupsDoRequest(http, request, "/admin/"));

    if (cupsLastError() > IPP_STATUS_OK_CONFLICTING) {

        LOG_E(TAG, "Cannot enable printer:", cupsLastErrorString());

        return false;
    }

    request = ippNewRequest(IPP_OP_RESUME_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    ippDelete(cupsDoRequest(http, request, "/admin/"));

    if (cupsLastError() > IPP_STATUS_OK_CONFLICTING) {

        LOG_E(TAG, "Cannot resume printer:", cupsLastErrorString());

        return false;
    }

    LOG_D(TAG, "Printer resumed");

    return true;
}

bool PrinterManager::printImage() {
    if(!hasImagePrepared || printer_name.empty())
        return false;

    resumePrinter();

    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, printer_name.c_str(), "self-o-mat", 0, nullptr);

    if (job_id > 0) {
        LOG_D(TAG, "successfully created the print job");
        cupsStartDocument(CUPS_HTTP_DEFAULT, printer_name.c_str(), job_id, "my_image", "image/png", 1);


        cupsWriteRequestData(CUPS_HTTP_DEFAULT, (const char *) imageTmpBuffer, sizeOfPreparedImage);

        cupsFinishDocument(CUPS_HTTP_DEFAULT, printer_name.c_str());
        return true;
    }
    return false;
}

PrinterManager::PrinterManager(ILogger *logger) : logger(logger) {
    currentPrinterState = STATE_UNKNOWN;
}

bool PrinterManager::prepareImageForPrint(cv::Mat image) {
    // Write image to blob
    std::vector<uchar> pngdata;

    cv::imencode(".png", image, pngdata);

    // copy the blob to our buffer for later use
    selfomat::tools::requireBufferWithSize(&imageTmpBuffer, &imageTmpBufferSize, pngdata.size());

    memcpy(imageTmpBuffer, pngdata.data(), pngdata.size());

    sizeOfPreparedImage = pngdata.size();
    hasImagePrepared = true;


    return true;
}

bool PrinterManager::cancelPrint() {
    hasImagePrepared = false;
    sizeOfPreparedImage = 0;
}

