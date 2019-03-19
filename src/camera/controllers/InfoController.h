//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H
#define SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H


#include "BaseController.h"

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class InfoController : public BaseController {

            private:
                CameraWidget *cameraNameWidget = nullptr;
                CameraWidget *lensNameWidget = nullptr;
            public:
                InfoController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

                std::string getCameraName();

                std::string getLensName();

                bool supportsCamera() override;

            };
        }
    }
}

#endif //SELF_O_MAT_GPHOTOCAMERAINFOCONTROLLER_H
