//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_APERTURECONTROLLER_H
#define SELF_O_MAT_APERTURECONTROLLER_H


#include "BaseController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ApertureController : public BaseController {
            private:
                static const std::string TAG;
                CameraWidget *p_apertureWidget = nullptr;
                int currentChoice;
                std::vector<std::string> choices;

            public:
                ApertureController(GPContext *gp, Camera *camera, CameraWidget *rootWidget);
                bool supportsCamera() override;

                const std::vector<std::string> * const getChoices();

                bool setAperture(int choice);

                bool pullSettings() override;
            };
        }
    }
}


#endif //SELF_O_MAT_APERTURECONTROLLER_H
