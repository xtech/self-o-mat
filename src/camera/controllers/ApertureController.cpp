//
// Created by clemens on 19.03.19.
//

#include "ApertureController.h"

using namespace selfomat::camera::gphoto;

const std::string ApertureController::TAG = "ApertureController";

ApertureController::ApertureController(GPContext *gp,
        Camera *camera, CameraWidget *rootWidget) : BaseController(gp, camera, rootWidget) {

    currentChoice = -1;

    if(!findWidget("aperture", &p_apertureWidget))
        findWidget("f-number", &p_apertureWidget);

    if(!loadChoices(p_apertureWidget, choices)) {
        LOG_E(TAG, "Error Loading aperture choices");
        // Clear the pointer so that we don't try to use it
        p_apertureWidget = nullptr;
    }

    pullSettings();
}

bool ApertureController::supportsCamera() {
    return p_apertureWidget != nullptr;
}

const std::vector<std::string> *const ApertureController::getChoices() {
    return &choices;
}

bool ApertureController::setAperture(int choice) {
    if(!supportsCamera())
        return false;
    if(choice < 0 || choice >= choices.size())
        return false;
    return setProperty(p_apertureWidget, choices, choice);
}

bool ApertureController::pullSettings() {
    if(!supportsCamera())
        return false;

    currentChoice = getProperty(p_apertureWidget, choices);

    if(currentChoice >= 0) {
        LOG_D(TAG, "Loaded aperture choice: " << choices[currentChoice]);
        resetDirty();
        return true;
    }
    LOG_E(TAG, "Error loading aperture choice");
    return false;
}

int ApertureController::getAperture() {
    return currentChoice;
}
