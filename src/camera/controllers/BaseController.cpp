//
// Created by clemens on 19.03.19.
//


#include "BaseController.h"

using namespace selfomat::camera::gphoto;

const std::string BaseController::TAG = "BaseController";

bool BaseController::findWidget(std::string widgetName, CameraWidget **target) {
    if(GP_OK == gp_widget_get_child_by_name(rootWidget, widgetName.c_str(), target)) {
        LOG_D(TAG, "Found widget: " << widgetName);
        return true;
    }
    LOG_D(TAG, "Did not find widget: " << widgetName);
    *target = nullptr;
    return false;
}

bool BaseController::loadChoices(CameraWidget *widget, std::vector<std::string> &choices) {
    choices.clear();

    int choice_count = gp_widget_count_choices(widget);

    for (int i = 0; i < choice_count; i++) {
        const char *choice;
        if (GP_OK == gp_widget_get_choice(widget, i, &choice)) {
            std::string choice_string;
            // Use assign to copy the string not just the ptr
            choice_string = choice;
            choices.push_back(choice_string);
        } else {
            return false;
        }
    }

    return true;
}

std::string BaseController::getProperty(CameraWidget *widget) {
    // Get the current choice
    const char *value;

    if (GP_OK != gp_widget_get_value(widget, &value)) {
        return "N/A";
    }

    std::string value_string = value;

    return value_string;
}

bool BaseController::setProperty(CameraWidget *widget, std::string value) {
    if(GP_OK != gp_widget_set_value(widget, value.c_str()))
        return false;
    dirty = true;
    return true;
}

bool BaseController::isDirty() {
    return dirty;
}

void BaseController::resetDirty() {
    dirty = false;
}
