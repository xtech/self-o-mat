//
// Created by clemens on 19.03.19.
//

#include "GphotoTriggerController.h"

using namespace selfomat::camera::gphoto;

const std::string GphotoTriggerController::TAG = "TriggerController";

GphotoTriggerController::GphotoTriggerController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : GphotoBaseController(gp, camera, rootWidget) {
    // Find the widgets we need in order to focus and trigger the camera
    LOG_D(TAG, "Looking for trigger widget");

    if(findWidget("eosremoterelease", &triggerWidget)) {
        mode = TRIGGER_MODE_EOSREMOTERELEASE;
        return;
    }

    mode = TRIGGER_MODE_DEFAULT;
}

bool GphotoTriggerController::trigger() {
    LOG_D(TAG, "Triggering");

    switch (mode) {
        case TRIGGER_MODE_DEFAULT: {
            bool success = GP_OK == gp_camera_trigger_capture(camera, gp);
            if (!success) {
                LOG_E(TAG, "Error triggering capture");
            }
            return success;
        }
        case TRIGGER_MODE_EOSREMOTERELEASE: {
            bool success = true;

            success &= (GP_OK == gp_widget_set_value(triggerWidget, "Immediate"));
            success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));

            if (!success) {
                LOG_E(TAG, "Error triggering capture");
            }
            return success;
        }
        default:
            LOG_E(TAG, "Invalid Trigger Mode");
            return false;
    }
}

bool GphotoTriggerController::releaseTrigger() {


    switch (mode) {
        case TRIGGER_MODE_DEFAULT:
            // No need
            return true;
        case TRIGGER_MODE_EOSREMOTERELEASE: {
            LOG_D(TAG, "Releasing Trigger");
            bool success = true;

            success &= (GP_OK == gp_widget_set_value(triggerWidget, "Release Full"));
            success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));
            success &= (GP_OK == gp_widget_set_value(triggerWidget, "None"));
            success &= (GP_OK == gp_camera_set_config(camera, rootWidget, gp));

            if (!success) {
                LOG_E(TAG, "Error releasing trigger");
            }
            return success;
        }
        default:
            LOG_E(TAG, "Invalid Trigger Mode");
            return false;
    }
}

bool GphotoTriggerController::supportsCamera() {
    // We hope so
    return true;
}
