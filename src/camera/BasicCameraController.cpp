//
// Created by clemens on 13.03.19.
//

#include "BasicCameraController.h"
const std::string BasicCameraController::TAG = "Basic Camera Controller";

BasicCameraController::BasicCameraController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : IGphotoController(gp, camera, rootWidget) {
    // Find the widgets we need in order to focus and trigger the camera

    LOG_D(TAG, "Looking for the focus widget");

    bool focusFound = (GP_OK == gp_widget_get_child_by_name(rootWidget, "autofocusdrive", &focusWidget));

    if(!focusFound)
        focusFound |= (GP_OK == gp_widget_get_child_by_name(rootWidget, "autofocus", &focusWidget));
    
    if(!focusFound) {
        LOG_D(TAG, "no focus widget available");
        focusWidget = nullptr;
        return;
    } else {
        LOG_D(TAG, "We have found the focus widget");
    }
}

bool BasicCameraController::supportsCamera() {
    // Let's hope so at least
    return true;
}

bool BasicCameraController::canTrigger() {
    // Let's hope gphoto does its job
    return true;
}

bool BasicCameraController::trigger() {
    auto retval = gp_camera_trigger_capture(camera, gp);
    LOG_D(TAG, "Triggering");
    if(retval != GP_OK) {
        LOG_E(TAG, "Error triggering capture");
        return false;
    }
    return true;
}

bool BasicCameraController::releaseTrigger() {
    // We don't need it
    return true;
}

bool BasicCameraController::canFocus() {
    return focusWidget != nullptr;
}

bool BasicCameraController::focus() {
    if (!canFocus())
        return false;
    LOG_D(TAG, "Starting Focus");
    int value = 1;
    auto retval = gp_widget_set_value(focusWidget, &value);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error triggering capture");
        return false;
    }

    retval = gp_camera_set_config(camera, rootWidget, gp);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error transmitting new value to cam with error " << gp_result_as_string(retval));
        return false;
    }

    return true;}

bool BasicCameraController::stopFocus() {
    if (!canFocus())
        return false;
    LOG_D(TAG, "Stopping Focus");
    int value = 0;
    auto retval = gp_widget_set_value(focusWidget, &value);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error triggering capture");
        return false;
    }

    retval = gp_camera_set_config(camera, rootWidget, gp);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error transmitting new value to cam with error " << gp_result_as_string(retval));
        return false;
    }

    return true;
}
