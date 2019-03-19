//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_IGPHOTOCONTROLLER_H
#define SELF_O_MAT_IGPHOTOCONTROLLER_H

#include <string>
#include <tools/verbose.h>

extern "C" {
#include <gphoto2/gphoto2.h>
}

class IGphotoController {
private:
    static const std::string TAG;

protected:
    GPContext *gp;
    Camera *camera;
    CameraWidget *rootWidget;

    bool findWidget(std::string widgetName, CameraWidget **target);

public:
    IGphotoController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) : gp(gp), camera(camera),
                                                                                 rootWidget(rootWidget) {}

    virtual ~IGphotoController() {
        // We do not free the camera here as we only use it.
    }

    /**
     * @return true, if camera is supported by this controller
     */
     virtual bool supportsCamera() = 0;
};

#endif //SELF_O_MAT_IGPHOTOCONTROLLER_H
