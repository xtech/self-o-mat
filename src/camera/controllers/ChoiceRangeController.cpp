//
// Created by clemens on 20.03.19.
//

#include "ChoiceRangeController.h"



using namespace selfomat::camera::gphoto;

const std::string ChoiceRangeController::TAG = "ChoiceRangeController";

ChoiceRangeController::ChoiceRangeController(GPContext *gp,
                                       Camera *camera, CameraWidget *rootWidget, std::vector<std::string> widgetNames) : BaseController(gp, camera, rootWidget) {
    currentChoice = -1;
    for(auto &widgetName : widgetNames)
        if(findWidget(widgetName, &p_choiceRangeWidget))
            break;
    if(p_choiceRangeWidget != nullptr) {
        // We have found a widget, load the choices
        if(!loadChoices(p_choiceRangeWidget, choices)) {
            LOG_E(TAG, "Error Loading choices");
            // Clear the pointer so that we don't try to use it
            p_choiceRangeWidget = nullptr;
        } else {
            // We have loaded the choices, pull latest settings
            pullSettings();
        }
    }
}

bool ChoiceRangeController::supportsCamera() {
    return p_choiceRangeWidget != nullptr;
}

const std::vector<std::string> *const ChoiceRangeController::getChoices() {
    return &choices;
}

bool ChoiceRangeController::setChoice(int choice) {
    if(!supportsCamera())
        return false;
    if(choice < 0 || choice >= choices.size())
        return false;

    if(setProperty(p_choiceRangeWidget, choices, choice)) {
        currentChoice = choice;
        return true;
    }
    return false;
}

bool ChoiceRangeController::pullSettings() {
    if(!supportsCamera())
        return false;

    currentChoice = getProperty(p_choiceRangeWidget, choices);

    if(currentChoice >= 0) {
        LOG_D(TAG, "Loaded choice: " << choices[currentChoice]);
        resetDirty();
        return true;
    }
    LOG_E(TAG, "Error loading current choice");
    return false;
}

int ChoiceRangeController::getChoice() {
    return currentChoice;
}

bool ChoiceRangeController::isReadOnly() {
    if(!supportsCamera())
        return true;
    int readonly = 1;
    if(GP_OK != gp_widget_get_readonly(p_choiceRangeWidget, &readonly)) {
        LOG_E(TAG, "Error getting readonly state");
        return true;
    }
    return readonly != 0;
}
