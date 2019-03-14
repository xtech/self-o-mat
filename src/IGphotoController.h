//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_IGPHOTOCONTROLLER_H
#define SELF_O_MAT_IGPHOTOCONTROLLER_H

extern "C" {
#include <gphoto2/gphoto2.h>
}

class IGphotoController {

protected:
    GPContext *gp;
    Camera *camera;
    CameraWidget *rootWidget;

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

    /**
     * @return true if this controller can trigger the camera
     */
    virtual bool canTrigger() = 0;

    /**
     * @return true if this controller can focus the camera
     */
    virtual bool canFocus() = 0;

    /**
     * Trigger the capture
     * @return true, if trigger was successful
     */
    virtual bool trigger() = 0;

    /**
     * Release the trigger
     * @return true, if trigger release was successful
     */
    virtual bool releaseTrigger() = 0;


    /**
     * Focus the camera
     * @return true, if focus was started successfully
     */
    virtual bool focus() = 0;

    /**
     * Stop focusing the camera
     * @return true, if focus was started successfully
     */
    virtual bool stopFocus() = 0;

};

#endif //SELF_O_MAT_IGPHOTOCONTROLLER_H
