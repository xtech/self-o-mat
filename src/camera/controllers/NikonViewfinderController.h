//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_NIKON_VIEWFINDER_CONTROLLER_H
#define SELF_O_MAT_NIKON_VIEWFINDER_CONTROLLER_H


#include "BaseController.h"

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class NikonViewfinderController : public BaseController {
            public:
                NikonViewfinderController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                        BaseController(gp, camera, rootWidget) {

                    findWidget("viewfinder", &viewfinderWidget);
                }

            private:
                CameraWidget *viewfinderWidget = nullptr;


            public:
                bool setViewfinder(bool enabled) {
                    if (viewfinderWidget != nullptr) {
                        LOG_I("NikonViewfinderController", "setting viewfinder to ", std::to_string(enabled));
                        return setPropertyToggle(viewfinderWidget, enabled);
                    }
                    return false;
                }

                bool supportsCamera() override {
                    return viewfinderWidget != nullptr;
                }

                bool pullSettings() override {
                    return true;
                }
            };
        }
    }
}


#endif //SELF_O_MAT_IsoController_H
