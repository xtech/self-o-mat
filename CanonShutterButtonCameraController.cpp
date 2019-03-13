//
// Created by clemens on 13.03.19.
//

#include "CanonShutterButtonCameraController.h"

const std::string CanonShutterButtonCameraController::TAG = "Canon ShutterButtonTrigger";

CanonShutterButtonCameraController::CanonShutterButtonCameraController(GPContext *gp, Camera *camera,
                                                                       CameraWidget *rootWidget) : BasicCameraController(gp,
                                                                                                                     camera,
                                                                                                                     rootWidget) {
    // Find the widgets we need in order to focus and trigger the camera
    LOG_D(TAG, "Looking for trigger widget");

    if (GP_OK == gp_widget_get_child_by_name(rootWidget, "eosremoterelease", &triggerWidget)) {
        LOG_D(TAG, "We have found the trigger widget");
    } else {
        LOG_D(TAG, "no trigger widget available");
        triggerWidget = nullptr;
        return;
    }
}

bool CanonShutterButtonCameraController::canTrigger() {
    return triggerWidget != nullptr;
}


bool CanonShutterButtonCameraController::trigger() {
    if (!canTrigger())
        return false;
    LOG_D(TAG, "Pressing Trigger");
    auto retval = gp_widget_set_value(triggerWidget, "Immediate");
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

bool CanonShutterButtonCameraController::releaseTrigger() {
    if (!canTrigger())
        return false;
    LOG_D(TAG, "Releasing Trigger");
    auto retval = gp_widget_set_value(triggerWidget, "Release Full");
    if (retval != GP_OK) {
        LOG_E(TAG, "Error releasing trigger");
        return false;
    }

    retval = gp_camera_set_config(camera, rootWidget, gp);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error transmitting new value to cam with error " << gp_result_as_string(retval));
        return false;
    }
    retval = gp_widget_set_value(triggerWidget, "None");
    if (retval != GP_OK) {
        LOG_E(TAG, "Error releasing trigger");
        return false;
    }

    retval = gp_camera_set_config(camera, rootWidget, gp);
    if (retval != GP_OK) {
        LOG_E(TAG, "Error transmitting new value to cam with error " << gp_result_as_string(retval));
        return false;
    }

    return true;
}

bool CanonShutterButtonCameraController::supportsCamera() {
    return canTrigger();
}


CanonShutterButtonCameraController::~CanonShutterButtonCameraController() = default;



