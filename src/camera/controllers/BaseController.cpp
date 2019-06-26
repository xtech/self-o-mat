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

    CameraWidgetType type;
    if(GP_OK != gp_widget_get_type(widget, &type)) {
        LOG_E(TAG, "Error getting type for widget");
        return false;
    }

    switch (type) {
        case GP_WIDGET_RADIO: {
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
        case GP_WIDGET_RANGE: {
            float min, max, step;
            if(GP_OK != gp_widget_get_range(widget, &min, &max, &step)) {
                LOG_E(TAG, "Error getting range parameters");
                return false;
            }
            LOG_D(TAG, "Range Properties: min= " << min << ", max = " << max << ", step = " << step);

            float current = min;
            do {
                choices.push_back(std::to_string(current));
                current+=step;
            }while(current <= max);
            return true;
        }
        default:
            return false;
    }
}

int BaseController::getProperty(CameraWidget *widget, std::vector<std::string> &choices) {

    CameraWidgetType type;
    if(GP_OK != gp_widget_get_type(widget, &type)) {
        LOG_E(TAG, "Error getting type for widget");
        return -1;
    }

    switch(type) {
        case GP_WIDGET_RADIO: {
            // Get the current choice
            const char *value;

            if (GP_OK != gp_widget_get_value(widget, &value)) {
                return -1;
            }

            std::string value_string = value;

            for(int i = 0; i < choices.size(); i++)
                if(choices[i] == value_string)
                    return i;
            return -1;
        }
        case GP_WIDGET_RANGE: {
            // it's float
            float value;

            float min, max, step;
            if(GP_OK != gp_widget_get_range(widget, &min, &max, &step))
                return -1;

            if (GP_OK != gp_widget_get_value(widget, &value))
                return -1;

            int choice = static_cast<int>((value - min) / (max - min));

            if(choice < 0 || choice >= choices.size())
                return -1;

            return choice;
        }
        default:
            return -1;
    }

}

bool BaseController::setProperty(CameraWidget *widget, std::vector<std::string> &choices, int choice) {

    CameraWidgetType type;
    if(GP_OK != gp_widget_get_type(widget, &type)) {
        LOG_E(TAG, "Error getting type for widget");
        return false;
    }

    switch (type) {
        case GP_WIDGET_RADIO:
            if(GP_OK != gp_widget_set_value(widget, choices[choice].c_str()))
                return false;
            dirty = true;
            return true;
        case GP_WIDGET_RANGE: {
            float min, max, step;
            if (GP_OK != gp_widget_get_range(widget, &min, &max, &step)) {
                LOG_E(TAG, "Error getting range parameters");
                return false;
            }
            float floatValue = min + choice * step;

            if (floatValue < min || floatValue > max) {
                LOG_E(TAG, "Invalid value: " << floatValue << "; min = " << min << ", max = " << max << ", step: " << step);
                return false;
            }

            // set the value
            if(GP_OK != gp_widget_set_value(widget, &floatValue))
                return false;

            dirty = true;
            return true;
        }
        default:
            return false;
    }
}

bool BaseController::setPropertyToggle(CameraWidget *widget, bool value) {
    CameraWidgetType type;
    if(GP_OK != gp_widget_get_type(widget, &type)) {
        LOG_E(TAG, "Error getting type for widget");
        return false;
    }


    if(type != GP_WIDGET_TOGGLE)
        return false;

    if(GP_OK != gp_widget_set_value(widget, &value))
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

std::string BaseController::getPropertyText(CameraWidget *widget) {
    // Get the current choice
    const char *value;

    if (GP_OK != gp_widget_get_value(widget, &value)) {
        return "N/A";
    }

    std::string value_string = value;

    return value_string;
}

