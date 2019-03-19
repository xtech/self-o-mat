//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H
#define SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H


#include "GphotoBaseController.h"

class GphotoCameraInfoController : public GphotoBaseController {

private:
    CameraWidget *cameraNameWidget = nullptr;
    CameraWidget *lensNameWidget = nullptr;
public:
    GphotoCameraInfoController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

    std::string getCameraName();
    std::string getLensName();

    bool supportsCamera() override;

};


#endif //SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H
