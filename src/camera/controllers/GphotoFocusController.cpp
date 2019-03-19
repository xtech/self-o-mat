//
// Created by clemens on 19.03.19.
//

#include "GphotoFocusController.h"

const std::string GphotoFocusController::TAG = "FocusController";

GphotoFocusController::GphotoFocusController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : GphotoBaseController(gp, camera, rootWidget) {
    // Find the widgets we need in order to focus and trigger the camera
    LOG_D(TAG, "Looking for focus widget");

    if(findWidget("autofocusdrive", &focusWidget))
        return;
    if(findWidget("autofocus", &focusWidget))
        return;
    LOG_D(TAG, "No focus widget found.");
}

bool GphotoFocusController::focus() {
    if (!supportsCamera())
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

    return true;
}

bool GphotoFocusController::stopFocus() {
    if (!supportsCamera())
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

bool GphotoFocusController::supportsCamera() {
    return focusWidget != nullptr;
}
