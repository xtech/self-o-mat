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

    cups_dest_t *dest = NULL;

    int printer_state = -1;
    const char *printer_state_reasons = NULL;

    dest = cupsGetDest("Brother_HL-L2370DN_series",
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

    std::cout << "Current printer state: " << currentPrinterState << std::endl;
    std::cout << "Reasons: " << printer_state_reasons << std::endl;

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

