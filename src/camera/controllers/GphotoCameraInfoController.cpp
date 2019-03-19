//
// Created by clemens on 19.03.19.
//

#include "GphotoCameraInfoController.h"

using namespace selfomat::camera::gphoto;

GphotoCameraInfoController::GphotoCameraInfoController(GPContext *gp, Camera *camera, CameraWidget *rootWidget)
        : GphotoBaseController(gp, camera, rootWidget) {
    findWidget("cameramodel", &cameraNameWidget);
    findWidget("lensname", &lensNameWidget);
}

std::string GphotoCameraInfoController::getCameraName() {
    if(cameraNameWidget == nullptr)
        return "N/A";
    return getProperty(cameraNameWidget);
}

std::string GphotoCameraInfoController::getLensName() {
    if(lensNameWidget == nullptr)
        return "N/A";
    return getProperty(lensNameWidget);
}

bool GphotoCameraInfoController::supportsCamera() {
    // Worst case we return "N/A"
    return true;
}

