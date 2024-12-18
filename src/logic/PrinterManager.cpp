#include <utility>
#include <algorithm>

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

        httpClose(http);

        return false;
    }

    request = ippNewRequest(IPP_OP_RESUME_PRINTER);

    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());

    ippDelete(cupsDoRequest(http, request, "/admin/"));

    httpClose(http);

    if (cupsLastError() > IPP_STATUS_OK_CONFLICTING) {

        LOG_E(TAG, "Cannot resume printer:", cupsLastErrorString());

        return false;
    }

    LOG_D(TAG, "Printer resumed");

    return true;
}

int PrinterManager::printImage() {
    if(!hasImagePrepared || printer_name.empty())
        return 0;

    resumePrinter();

    // Job ID or 0 on error
    int job_id = cupsCreateJob(CUPS_HTTP_DEFAULT, printer_name.c_str(), "self-o-mat", 0, nullptr);

    if (job_id > 0) {
        LOG_D(TAG, "successfully created the print job");
        cupsStartDocument(CUPS_HTTP_DEFAULT, printer_name.c_str(), job_id, "my_image", "image/png", 1);

        cupsWriteRequestData(CUPS_HTTP_DEFAULT, (const char *) imageTmpBuffer, sizeOfPreparedImage);

        cupsFinishDocument(CUPS_HTTP_DEFAULT, printer_name.c_str());
    }
    return job_id;
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
    return true;
}

bool PrinterManager::getJobDetails(int jobId, PrinterJobState &state, time_t &creationTs, time_t &processingTs, time_t &completedTs) {
    if(jobId <= 0)
        return false;

    boost::unique_lock<boost::mutex> lk(printerStateMutex);

    cups_job_t *cupsJobs;
    int cupsJobCount = cupsGetJobs2(CUPS_HTTP_DEFAULT, &cupsJobs, NULL, 0, CUPS_WHICHJOBS_ALL);
    //LOG_D(TAG, "Number of print jobs: ", std::to_string(cupsJobCount));

    for(int i = 0; i < cupsJobCount; i++) {
        if (cupsJobs[i].id == jobId) {
            //LOG_D(TAG, "Found print job with ID #", std::to_string(jobId));

            creationTs = cupsJobs[i].creation_time;
            processingTs = cupsJobs[i].processing_time;
            completedTs = cupsJobs[i].completed_time;

            switch(cupsJobs[i].state)
            {
                // map known values
                case IPP_JOB_PENDING:
                    state = JOB_STATE_PENDING;
                    break;
                case IPP_JOB_HELD:
                    state = JOB_STATE_HELD;
                    break;
                case IPP_JOB_PROCESSING:
                    state = JOB_STATE_PROCESSING;
                    break;
                case IPP_JOB_STOPPED:
                    state = JOB_STATE_STOPPED;
                    break;
                case IPP_JOB_CANCELLED:
                    state = JOB_STATE_CANCELED;
                    break;
                case IPP_JOB_ABORTED:
                    state = JOB_STATE_ABORTED;
                    break;
                case IPP_JOB_COMPLETED:
                    state = JOB_STATE_COMPLETED;
                    break;
                default:
                    state = JOB_STATE_UNKNOWN;
                    break;
            }
            return true;
        }
    }

    return false;
}

const char* PrinterManager::printerJobStateToString(PrinterJobState &state) {
    switch(state)
    {
	// map known values
        case JOB_STATE_PENDING:
            return "pending";
        case JOB_STATE_HELD:
            return "held";
        case JOB_STATE_PROCESSING:
            return "processing";
        case JOB_STATE_STOPPED:
            return "stopped";
        case JOB_STATE_CANCELED:
            return "canceled";
        case JOB_STATE_ABORTED:
            return "aborted";
        case JOB_STATE_COMPLETED:
            return "completed";
    	case JOB_STATE_UNKNOWN:
	default:
	    return "unknown";
    }
}

void PrinterManager::checkPrinterAttentionFromJob(int jobId, unsigned int &flags) {
    if(jobId <= 0)
        return;

    boost::unique_lock<boost::mutex> lk(printerStateMutex);

    //LOG_D(TAG, "Querying attributes for print job with ID #", std::to_string(jobId));

    http_t *http;
    ipp_t *request;
    ipp_t *response;
    char uri[HTTP_MAX_URI];
    char job_printer_state_reasons[2048];
    ipp_attribute_t *attr;

    http = httpConnect2(cupsServer(),
                        ippPort(),
                        NULL,
                        AF_UNSPEC,
                        cupsEncryption(),
                        1,
                        30000,
                        NULL);


    httpAssembleURIf(HTTP_URI_CODING_ALL, uri, sizeof(uri), "ipp", NULL, "localhost", ippPort(), "/printers/%s", printer_name.c_str());

    request = ippNewRequest(IPP_OP_GET_JOB_ATTRIBUTES);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_URI, "printer-uri", NULL, uri);
    ippAddInteger(request, IPP_TAG_OPERATION, IPP_TAG_INTEGER, "job-id", jobId);
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_NAME, "requesting-user-name", NULL, cupsUser());
    ippAddString(request, IPP_TAG_OPERATION, IPP_TAG_KEYWORD, "requested-attributes", NULL, "job-printer-state-reasons");

    response = cupsDoRequest(http, request, "/admin");

    job_printer_state_reasons[0] = '\0';
    if ((attr = ippFindAttribute(response, "job-printer-state-reasons", IPP_TAG_KEYWORD)) != NULL) {
        ippAttributeString(attr, job_printer_state_reasons, sizeof(job_printer_state_reasons));
    }

    ippDelete(response);

    httpClose(http);

    std::vector<std::string> needsPaperReasons = {"media-empty-error", "media-needed"};
    std::vector<std::string> needsInkReasons = {"marker-supply-empty-error"};
    std::string inputTrayMissingReason = "input-tray-missing";
    std::vector<std::string> reasons;

    boost::split(reasons,
                 job_printer_state_reasons,
                 boost::is_any_of(", "),
                 boost::token_compress_on);

    for (std::vector<std::string>::iterator r = reasons.begin(); r != reasons.end(); ++r) {
        LOG_D(TAG, "Job printer state reason: ", *r);
        if (std::find(needsPaperReasons.begin(), needsPaperReasons.end(), *r) != needsPaperReasons.end()) {
            LOG_D(TAG, "Printer needs paper!");
	    flags |= PRINTER_ATTN_NO_PAPER;
	}
        if (std::find(needsInkReasons.begin(), needsInkReasons.end(), *r) != needsInkReasons.end()) {
            LOG_D(TAG, "Printer needs ink!");
	    flags |= PRINTER_ATTN_NO_INK;
        }
	if (inputTrayMissingReason.compare(*r) == 0) {
            LOG_D(TAG, "Printer needs input tray!");
	    flags |= PRINTER_ATTN_NO_TRAY;
        }
    }
}
