#include <utility>

//
// Created by clemens on 12.02.19.
//

#include "PrinterManager.h"

using namespace selfomat::logic;

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

                std::cout << "Found Cups devices: " << device_uri << std::endl;


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
                    std::cout << "Found default printer: " << printerName << std::endl;
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
            std::cout << "Removed old default printer" << std::endl;
            printer_name.clear();
        }
    }

    if (printer_name.empty() && cupsDestinationCount > 0) {
        std::cout << "Found any printer: " << cupsDestinations[0].name << std::endl;
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
        std::cout << "Current printer state: " << currentPrinterState << std::endl;
        for (auto i: currentStateReasons)
            std::cout << i << std::endl;
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

        std::cout << "Cannot enable printer:" << cupsLastErrorString() << std::endl;

        return false;
    }

    request = ippNewRequest(IPP_OP_RESUME_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    ippDelete(cupsDoRequest(http, request, "/admin/"));

    if (cupsLastError() > IPP_STATUS_OK_CONFLICTING) {

        std::cout << "Cannot resume printer:" << cupsLastErrorString() << std::endl;

        return false;
    }

    std::cout << "Printer resumed." << std::endl;

    return true;
}

bool PrinterManager::printImage() {
    if(!hasImagePrepared || printer_name.empty())
        return false;

    resumePrinter();

    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, printer_name.c_str(), "self-o-mat", 0, nullptr);

    if (job_id > 0) {
        std::cout << "successfully created the job" << std::endl;
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

bool PrinterManager::prepareImageForPrint(Magick::Image image) {
    // Write image to blob
    image.magick("png");
    Magick::Blob blob;
    image.write(&blob);

    // copy the blob to our buffer for later use
    selfomat::tools::requireBufferWithSize(&imageTmpBuffer, &imageTmpBufferSize, blob.length());

    memcpy(imageTmpBuffer, blob.data(), blob.length());

    sizeOfPreparedImage = blob.length();
    hasImagePrepared = true;

    return true;
}

bool PrinterManager::cancelPrint() {
    hasImagePrepared = false;
    sizeOfPreparedImage = 0;
}

