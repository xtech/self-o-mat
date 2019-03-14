//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_CANONSHUTTERBUTTONCAMERACONTROLLER_H
#define SELF_O_MAT_CANONSHUTTERBUTTONCAMERACONTROLLER_H


#include <string>
#include "BasicCameraController.h"
#include "../tools/verbose.h"

class CanonShutterButtonCameraController : public BasicCameraController {

private:
    static const std::string TAG;

    CameraWidget *triggerWidget = nullptr;


public:
    CanonShutterButtonCameraController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

    virtual ~CanonShutterButtonCameraController();

    bool supportsCamera() override;

    bool canTrigger() override;

    bool trigger() override;


    bool releaseTrigger() override;
};


#endif //SELF_O_MAT_CANONSHUTTERBUTTONCAMERACONTROLLER_H
