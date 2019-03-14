//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_BASICCAMERACONTROLLER_H
#define SELF_O_MAT_BASICCAMERACONTROLLER_H


#include <string>
#include "IGphotoController.h"
#include "../tools/verbose.h"

class BasicCameraController : public IGphotoController {
private:
    static const std::string TAG;

    CameraWidget *focusWidget = nullptr;

public:
    BasicCameraController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

    bool canFocus() override;

    bool focus() override;

    bool supportsCamera() override;

    bool canTrigger() override;

    bool trigger() override;

    bool releaseTrigger() override;

    bool stopFocus() override;
};


#endif //SELF_O_MAT_BASICCAMERACONTROLLER_H
