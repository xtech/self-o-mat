//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H
#define SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H


#include <camera/controllers/IGphotoController.h>

class GphotoFocusController : public IGphotoController {
    static const std::string TAG;

private:
    bool focused = false;
    CameraWidget *focusWidget = nullptr;


public:
    GphotoFocusController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

    bool supportsCamera() override;

    bool focus();

    bool stopFocus();

};


#endif //SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H
