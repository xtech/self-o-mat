//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_GPHOTOTRIGGERCONTROLLER_H
#define SELF_O_MAT_GPHOTOTRIGGERCONTROLLER_H


#include "IGphotoController.h"
#include <string>
#include <tools/verbose.h>

enum TRIGGER_CONTROLLER_MODE {
    // Normal Gphoto2 trigger capture
    TRIGGER_MODE_DEFAULT = 0,
    // We use canon's eosremoterelease (it does not force autofocus)
    TRIGGER_MODE_EOSREMOTERELEASE = 1
};

class GphotoTriggerController : public IGphotoController {
    static const std::string TAG;

private:
    TRIGGER_CONTROLLER_MODE mode;
    CameraWidget *triggerWidget = nullptr;


public:
    GphotoTriggerController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

    bool supportsCamera() override;

    bool trigger();

    bool releaseTrigger();
};


#endif //SELF_O_MAT_GPHOTOTRIGGERCONTROLLER_H
