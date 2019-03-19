//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H
#define SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H


#include <camera/controllers/BaseController.h>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class FocusController : public BaseController {
                static const std::string TAG;

            private:
                bool focused = false;
                CameraWidget *focusWidget = nullptr;


            public:
                FocusController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);

                bool supportsCamera() override;

                bool focus();

                bool stopFocus();

                bool pullSettings() override;
            };
        }
    }
}


#endif //SELF_O_MAT_GPHOTOFOCUSCONTROLLER_H
