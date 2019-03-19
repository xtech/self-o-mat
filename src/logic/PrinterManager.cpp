#include <utility>

//
// Created by clemens on 12.02.19.
//

#include "PrinterManager.h"

using namespace selfomat::logic;

bool PrinterManager::start() {
    refreshCupsDestinations();
    refreshPrinterState();
    return true;
}



bool PrinterManager::refreshCupsDestinations() {
    cupsFreeDests(cupsDestinationCount, cupsDestinations);
    cupsDestinationCount = 0;

    cupsEnumDests(CUPS_DEST_FLAGS_NONE, 1000, nullptr, 0,0,
            (cups_dest_cb_t) [] (void *user_data, unsigned flags, cups_dest_t *dest) {
                auto *printerManager = (PrinterManager*)user_data;
                printerManager->cupsDestinationCount = cupsCopyDest(dest, printerManager->cupsDestinationCount, &printerManager->cupsDestinations);
                return 1;
        }, this);


    for(int i = 0; i < cupsDestinationCount; i++)
        std::cout << "Found Cups destination: " << cupsDestinations[i].name << std::endl;

    return true;
}

bool PrinterManager::refreshPrinterState() {

    static const char * const requested_attributes[] = {
            "printer-state",
            "printer-state-reasons"
    };

    char resource[256];
    http_t *http;
    const char *printer_uri;

    ipp_t           *request;
    ipp_t           *response;

    ipp_attribute_t *attr;

    cups_dest_t *dest = NULL;

    dest = cupsGetDest("Brother_HL-L2370DN_series",
                       NULL,
                       cupsDestinationCount,
                       cupsDestinations);

    http = cupsConnectDest(dest, CUPS_DEST_FLAGS_DEVICE,
                            30000, NULL, resource,
                            sizeof(resource), NULL, NULL);

    if (httpGetFd(http) >= 0) {

        request = ippNewRequest(IPP_OP_GET_PRINTER_ATTRIBUTES);

        printer_uri = cupsGetOption("device-uri",
                                    dest->num_options,
                                    dest->options);

        ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI,
                     "printer-uri", NULL, printer_uri);

        ippAddStrings(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD,
                      "requested-attributes", 2, NULL,
                      requested_attributes);


        response = cupsDoRequest(http, request, resource);

        if (cupsLastError() < IPP_STATUS_ERROR_BAD_REQUEST) {
            if ((attr = ippFindAttribute(response, "printer-state", IPP_TAG_ENUM)) != NULL) {

                switch(ippGetInteger(attr, 0)) {
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

            }

            if ((attr = ippFindAttribute(response, "printer-state-reasons", IPP_TAG_KEYWORD)) != NULL) {
                int i, count = ippGetCount(attr);

                currentStateReasons.clear();

                for (i = 0; i < count; i ++) {
                    currentStateReasons.insert( currentStateReasons.end(), ippGetString(attr, i, NULL));
                }
            }
        } else {
            currentPrinterState = STATE_UNKNOWN;
            currentStateReasons.clear();
        }



        ippDelete(response);

    }

    httpClose(http);

    std::cout << "Printer State: " << currentPrinterState << std::endl;
    for (auto i: currentStateReasons)
        std::cout << i << std::endl;

}

bool PrinterManager::printImage() {
    if(!hasImagePrepared)
        return false;
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

PrinterManager::PrinterManager(ILogger *logger, std::string printer_name) : logger(logger), printer_name(
        std::move(printer_name)) {}

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

