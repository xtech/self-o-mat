//
// Created by clemens on 19.03.19.
//


#include "IGphotoController.h"

const std::string IGphotoController::TAG = "IGphotoController";

bool IGphotoController::findWidget(std::string widgetName, CameraWidget **target) {
    if(GP_OK == gp_widget_get_child_by_name(rootWidget, widgetName.c_str(), target)) {
        LOG_D(TAG, "Found widget: " << widgetName);
        return true;
    }
    LOG_D(TAG, "Did not find widget: " << widgetName);
    *target = nullptr;
    return false;
}
