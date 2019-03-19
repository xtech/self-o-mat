//
// Created by clemens on 19.03.19.
//

#include "InfoController.h"

using namespace selfomat::camera::gphoto;

InfoController::InfoController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : BaseController(gp, camera, rootWidget) {
    findWidget("cameramodel", &cameraNameWidget);
    findWidget("lensname", &lensNameWidget);
}

std::string InfoController::getCameraName() {
    if(cameraNameWidget == nullptr)
        return "N/A";
    return getProperty(cameraNameWidget);
}

std::string InfoController::getLensName() {
    if(lensNameWidget == nullptr)
        return "N/A";
    return getProperty(lensNameWidget);
}

bool InfoController::supportsCamera() {
    // Worst case we return "N/A"
    return true;
}

bool InfoController::pullSettings() {
    return true;
}

