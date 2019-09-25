//
// Created by clemens on 21.01.19.
//


#include "GphotoCamera.h"

using namespace selfomat::camera;
using namespace selfomat::camera::gphoto;

const std::string GphotoCamera::TAG = "GPHOTO CAMERA";


CameraStartResult GphotoCamera::start() {

    lastDrainTime = boost::posix_time::min_date_time;
    focusStartedTime = boost::posix_time::min_date_time;


    const char **gphotoVersion = gp_library_version(GP_VERSION_VERBOSE);
    LOG_D(TAG, "GPhoto Version: ", (*gphotoVersion));

    char * camlibsenv = getenv("CAMLIBS");
    if(camlibsenv != nullptr) {
        LOG_D(TAG, "Camlibspath is: ", camlibsenv);
    } else {
        LOG_D(TAG, "Camlibspath is null");
    }


    gp = gp_context_new();

    CameraList *list;
    gp_list_new(&list);
    gp_camera_autodetect(list, gp);

    // Print the cameras
    int count = gp_list_count(list);
    LOG_I(TAG, std::string("Found ") + std::to_string( count) + " cameras.");
    gp_list_free(list);

    gp_camera_new(&camera);


    int retval = gp_camera_init(camera, gp);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error opening camera, error", gp_result_as_string(retval));
        setState(STATE_ERROR);
        return START_RESULT_ERROR;
    }

    LOG_I(TAG, "Successfully opened camera!");

    gp_camera_get_config(camera, &rootWidget, gp);


    // We have the root widget, find a controller which works with the connected camera
    if(!createCameraControllers()) {
        LOG_E(TAG, "Could not find a controller for the connected camera.");
        return START_RESULT_ERROR;
    }


    setState(STATE_WORKING);

    drainEventQueue(false);


    LOG_I(TAG, std::string("We're running with a ") + getCameraName() + " with " + getLensName() + " attached.");


    // For nikon cameras we try to find the viewfinder widget. if we have it, we make sure that the mirror is positioned correctly
    findWidget("viewfinder", &viewfinderWidget);
    if(viewfinderWidget != nullptr) {
        int value = 1;
        gp_widget_set_value(viewfinderWidget, &value);
        // push the config
        gp_camera_set_config(camera, rootWidget, gp);
        drainEventQueue(false);
    }

    return START_RESULT_SUCCESS;
}


void GphotoCamera::stop() {
    LOG_I(TAG, "Stopping camera!");
    // Forbid new capture or preview events
    setState(STATE_STOPPED);

    // drain the queue
    cameraIoMutex.lock();
    drainEventQueue(false);
    // For nikon cameras we make sure that the mirror is in the correct position
    if(viewfinderWidget != nullptr) {
        int value = 0;
        gp_widget_set_value(viewfinderWidget, &value);
        // push the config
        gp_camera_set_config(camera, rootWidget, gp);
        drainEventQueue(false);
    }
    cameraIoMutex.unlock();

    // free the widgets
    if (rootWidget != nullptr) {
        gp_widget_free(rootWidget);
        rootWidget = nullptr;
    }

    if (camera != nullptr && gp != nullptr) {
        gp_camera_exit(camera, gp);
    }

    if (camera != nullptr) {
        gp_camera_free(camera);
        camera = nullptr;
    }

    if (gp != nullptr) {
        gp_context_unref(gp);
        gp = nullptr;
    }

    // Free the buffers
    if (latestBuffer != nullptr) {
        free(latestBuffer);
        latestBuffer = nullptr;
    }
    if (tmpBuffer != nullptr) {
        free(tmpBuffer);
        tmpBuffer = nullptr;
    }
}

void GphotoCamera::drainEventQueue(bool waitForPhoto) {
    lastDrainTime = boost::posix_time::microsec_clock::local_time();
    CameraEventType evtype;
    void *data = nullptr;
    int retval;

    while (true) {
        retval = gp_camera_wait_for_event(camera, 10, &evtype, &data, gp);
        if (retval != GP_OK) {
            if (data != nullptr) {
                free(data);
            }
            LOG_E(TAG, "return from waitevent in trigger sample with ", gp_result_as_string(retval));
            return;
        }
        switch (evtype) {
            case GP_EVENT_TIMEOUT:
                free(data);
                // We're done if we don't need a photo
                if (!waitForPhoto)
                    return;
                break;
            case GP_EVENT_FILE_ADDED: {
                LOG_I(TAG, "Event FILE ADDED");
                CameraFile *cameraFile = nullptr;
                gp_file_new(&cameraFile);

                auto *path = static_cast<CameraFilePath *>(data);
                retval = gp_camera_file_get(camera, path->folder, path->name, CameraFileType::GP_FILE_TYPE_NORMAL,
                                            cameraFile, gp);
                if (retval != GP_OK) {
                    LOG_E(TAG, "Error obtaining camera file handle: ", gp_result_as_string(retval));
                    free(data);
                    gp_file_free(cameraFile);
                    continue;
                }

                LOG_D(TAG, "filename was: ",  path->name);

                if (boost::iends_with(path->name, ".JPG")) {
                    LOG_D(TAG, "We got the JPG!");



                    // release the trigger if still pressed
                    triggerController->releaseTrigger();

                    // reset exposure
                    exposureCorrectionController->postTrigger();
                    pushCameraSettings();

                    const char *imageData = nullptr;
                    unsigned long int imageDataSize;
                    // Get the data. This is jpeg
                    retval = gp_file_get_data_and_size(cameraFile, &imageData, &imageDataSize);

                    if (retval != GP_OK) {
                        free(data);
                        gp_file_free(cameraFile);
                        LOG_E(TAG, "Error fetching image data: ", gp_result_as_string(retval));
                        continue;
                    }

                    // Copy the data into our latest buffer
                    selfomat::tools::requireBufferWithSize(&latestBuffer, &latestBufferSize, imageDataSize);
                    memcpy(latestBuffer, imageData, imageDataSize);
                    latestFileName = path->name;

                    if (waitForPhoto) {
                        gp_file_free(cameraFile);

                        // delete the file from the camera
                        retval = gp_camera_file_delete(camera, path->folder, path->name, gp);

                        free(data);

                        waitForPhoto = false;
                        continue;
                    }
                } else {
                    LOG_I(TAG, "Got some other file. name was: ", path->name);

                    const char *imageData = nullptr;
                    unsigned long int imageDataSize;
                    // Get the data. This is some other file
                    retval = gp_file_get_data_and_size(cameraFile, &imageData, &imageDataSize);

                    if (retval != GP_OK) {
                        free(data);
                        gp_file_free(cameraFile);
                        LOG_E(TAG, "Error fetching image data: ", gp_result_as_string(retval));
                        continue;
                    }

                    // Copy the data into our latest buffer
                    {
                        unique_lock<boost::mutex> lk(rawBufferMutex);
                        selfomat::tools::requireBufferWithSize(&latestRawBuffer, &latestRawBufferSize, imageDataSize);
                        memcpy(latestRawBuffer, imageData, imageDataSize);
                        latestRawFileName = std::string(path->name);
                    }
                }

                gp_file_free(cameraFile);

                // delete the file from the camera
                retval = gp_camera_file_delete(camera, path->folder, path->name, gp);

                free(data);

            }
                break;
            default:
                LOG_D(TAG, "some other event. Type was:", std::to_string(evtype));
                // Free the data and do nothing
                free(data);
                break;
        }
    }
}

void GphotoCamera::drainEventQueueWhenNeeded() {
    auto now = boost::posix_time::microsec_clock::local_time();
    auto timeSinceLastDrain = (now - lastDrainTime).total_milliseconds();

    if (timeSinceLastDrain > 1000) {
        drainEventQueue(false);
    }
}

bool GphotoCamera::capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) {
    if (getState() != STATE_WORKING)
        return false;
    cameraIoMutex.lock();
    CameraFile *cameraFile = NULL;

    gp_file_new(&cameraFile);
    int retval = gp_camera_capture_preview(camera, cameraFile, gp);

    if (retval != GP_OK) {
        LOG_E(TAG, "Error capturing preview:", gp_result_as_string(retval));
        gp_file_free(cameraFile);
        drainEventQueueWhenNeeded();
        cameraIoMutex.unlock();
        return false;
    }

    const char *data;
    unsigned long int size;
    // Get the data. This is jpeg
    retval = gp_file_get_data_and_size(cameraFile, &data, &size);

    if (retval != GP_OK) {
        LOG_E(TAG, "Error fetching image data: ", gp_result_as_string(retval));
        cameraIoMutex.unlock();
        return false;
    }

    // It is already a preview image, do not scale.
    auto success = jpegDecoder.decodeJpeg((unsigned char *) data, size, &tmpBuffer, &tmpBufferSize, resultInfo, RGBA);

    // swap the buffers
    void *tmp = *buffer;
    size_t sizeTmp = *bufferSize;
    *buffer = tmpBuffer;
    *bufferSize = tmpBufferSize;
    tmpBuffer = tmp;
    tmpBufferSize = sizeTmp;

    gp_file_free(cameraFile);

    drainEventQueueWhenNeeded();

    if (settingsDirty()) {
        if (!pushCameraSettings()) {
            LOG_E(TAG, "Error pushing camera settings");
            pullCameraSettings();
        } else {
            LOG_D(TAG, "successfully pushed camera settings");
        }
    }

    if (trigger_focus) {
        focusController->focus();
        trigger_focus = false;
        focusStartedTime = boost::posix_time::microsec_clock::local_time();
        focus_active = true;
    }
    if (focus_active) {
        auto now = boost::posix_time::microsec_clock::local_time();
        auto focusActiveTime = (now - focusStartedTime).total_milliseconds();
        if (focusActiveTime > 3000) {
            focusController->stopFocus();
            focus_active = false;
        }
    }

    cameraIoMutex.unlock();

    return success;
}

bool GphotoCamera::pushCameraSettings() {
    bool success = (GP_OK == gp_camera_set_config(camera, rootWidget, gp));
    if(success) {
        for(auto *p_controller : registeredControllers)
            p_controller->resetDirty();
    }
    return success;
}

bool GphotoCamera::triggerCaptureBlocking() {
    if (getState() != STATE_WORKING)
        return false;

    cameraIoMutex.lock();

    // set exposure and trigger
    exposureCorrectionController->preTrigger();
    pushCameraSettings();
    bool success = triggerController->trigger();

    cameraIoMutex.unlock();

    return success;
}


int GphotoCamera::getIso() {
    return isoController->getChoice();
}

int GphotoCamera::getShutterSpeed() {
    return shutterController->getChoice();
}

int GphotoCamera::getAperture() {
    return apertureController->getChoice();
}

int GphotoCamera::getShootingMode() {
    return -1;
}

bool GphotoCamera::setIso(int iso_choice) {
    return isoController->setChoice(iso_choice);
}

bool GphotoCamera::setShutterSpeed(int shutter_speed_choice) {
    return shutterController->setChoice(shutter_speed_choice);
}

bool GphotoCamera::setAperture(int aperture_choice) {
    return apertureController->setChoice(aperture_choice);
}

void GphotoCamera::pullCameraSettings() {
    for(auto *p_controller : registeredControllers)
        p_controller->pullSettings();
}

const vector<string> * const GphotoCamera::getIsoChoices() {
    return isoController->getChoices();
}

const vector<string> * const GphotoCamera::getShutterSpeedChoices() {
    return shutterController->getChoices();
}

const vector<string> * const GphotoCamera::getApertureChoices() {
    return apertureController->getChoices();
}

const vector<string> * const GphotoCamera::getShootingModeChoices() {
    return nullptr;
}

bool GphotoCamera::autofocusBlocking() {
    trigger_focus = true;
}

const vector<string> * const GphotoCamera::getExposureCorrectionModeChoices() {
    return exposureCorrectionController->getChoices();
}

const vector<string> * const GphotoCamera::getImageFormatChoices() {
    return imageFormatController->getChoices();
}


string GphotoCamera::getCameraName() {
    return cameraInfoController->getCameraName();
}

string GphotoCamera::getLensName() {
    return cameraInfoController->getLensName();
}

int GphotoCamera::getExposureCorrection() {
    return exposureCorrectionController->getChoice();
}

int GphotoCamera::getExposureCorrectionTrigger() {
    return exposureCorrectionController->getTriggerChoice();
}

int GphotoCamera::getImageFormat() {
    return imageFormatController->getChoice();
}

bool GphotoCamera::setExposureCorrection(int exposure_correction_choice) {
    return exposureCorrectionController->setChoice(exposure_correction_choice);
}

bool GphotoCamera::setExposureCorrectionTrigger(int exposure_correction_choice) {
    return exposureCorrectionController->setTriggerChoice(exposure_correction_choice);
}

bool GphotoCamera::setImageFormat(int image_format_choice) {
    return imageFormatController->setChoice(image_format_choice);
}



bool GphotoCamera::readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, std::string *fullJpegFileName, void **previewBuffer,
                                     size_t *previewBufferSize, ImageInfo *previewImageInfo) {
    if (getState() != STATE_WORKING)
        return false;
    cameraIoMutex.lock();

    // Wait for the photo to arrive
    // TODO: maybe the camera is SO quick that we enter this stage after the photo was already processed. I doubt it tho
    drainEventQueue(true);

    // Jpeg is now in latestBuffer convert to bmp
    auto success = jpegDecoder.decodeJpeg((unsigned char *) latestBuffer, latestBufferSize, previewBuffer,
                           previewBufferSize,
                           previewImageInfo, JPEG_DECODE_COLORS::RGBA, 0, 800, LARGER_THAN_REQUIRED);

    LOG_D(TAG, "Preview image of the final shot has size: ", std::to_string(previewImageInfo->width) + "x" + std::to_string(previewImageInfo->height));


    // Swap the buffers for jpeg
    void *tmpBuffer = *fullJpegBuffer;
    size_t tmpBufferSize = *fullJpegBufferSize;

    *fullJpegBuffer = latestBuffer;
    *fullJpegBufferSize = latestBufferSize;
    *fullJpegFileName = latestFileName;


    latestBuffer = tmpBuffer;
    latestBufferSize = tmpBufferSize;

    cameraIoMutex.unlock();
    return success;
}

bool GphotoCamera::createCameraControllers() {

    triggerController = new TriggerController(gp, camera, rootWidget);
    registeredControllers.push_back(triggerController);
    focusController = new FocusController(gp, camera, rootWidget);
    registeredControllers.push_back(focusController);
    cameraInfoController = new InfoController(gp, camera, rootWidget);
    registeredControllers.push_back(cameraInfoController);
    apertureController = new ApertureController(gp, camera, rootWidget);
    registeredControllers.push_back(apertureController);
    imageFormatController = new ImageFormatController(gp, camera, rootWidget);
    registeredControllers.push_back(imageFormatController);
    isoController = new IsoController(gp, camera, rootWidget);
    registeredControllers.push_back(isoController);
    exposureCorrectionController = new ExposureCorrectionController(gp, camera, rootWidget);
    registeredControllers.push_back(exposureCorrectionController);
    shutterController = new ShutterSpeedController(gp, camera, rootWidget);
    registeredControllers.push_back(shutterController);

    return true;
}

bool GphotoCamera::getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) {
    unique_lock<boost::mutex> lk(rawBufferMutex);
    if(latestRawBuffer == nullptr) {
        return false;
    }

    *filename = latestRawFileName;
    *targetBuffer = latestRawBuffer;
    *targetSize = latestRawBufferSize;

    // null our buffer, caller has to free
    latestRawBuffer = nullptr;
    latestRawBufferSize = 0;

    return true;
}

GphotoCamera::~GphotoCamera() {
    for(auto controller : registeredControllers)
        delete(controller);
    registeredControllers.clear();
}

bool GphotoCamera::settingsDirty() {
    for(auto *p_controller: registeredControllers)
        if(p_controller->isDirty())
            return true;
    return false;
}

bool GphotoCamera::findWidget(std::string widgetName, CameraWidget **target) {
    if (GP_OK == gp_widget_get_child_by_name(rootWidget, widgetName.c_str(), target)) {
        LOG_D(TAG, "Found widget: ", widgetName);
        return true;
    }
    LOG_D(TAG, "Did not find widget: ", widgetName);
    *target = nullptr;
    return false;
}

