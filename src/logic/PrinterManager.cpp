#include <utility>

//
// Created by clemens on 12.02.19.
//

#include "PrinterManager.h"

using namespace selfomat::logic;

bool PrinterManager::start() {
    refreshCupsDestinations();
    refreshPrinterState();
    resumePrinter();
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

    boost::unique_lock<boost::mutex> lk(printerStateMutex);

    cups_dest_t     *dest = NULL;
    const char      *printer_state_reasons = NULL;
    int             printer_state = -1;


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
            currentPrinterState_ = STATE_IDLE;
            break;
        case 4 :
            currentPrinterState_ = STATE_PRINTING;
            break;
        case 5 :
            currentPrinterState_ = STATE_STOPPED;
            break;
        default:
            currentPrinterState_ = STATE_UNKNOWN;
    }

    currentStateReasons_.clear();

    if(printer_state_reasons == nullptr)
        return false;

    boost::split(currentStateReasons_,
                 printer_state_reasons,
                 boost::is_any_of(", "),
                 boost::token_compress_on);

    std::cout << "Current printer state: " << currentPrinterState_ << std::endl;
    for (auto i: currentStateReasons_)
        std::cout << i << std::endl;

    return true;
}

bool PrinterManager::resumePrinter() {

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
    if(!hasImagePrepared)
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

