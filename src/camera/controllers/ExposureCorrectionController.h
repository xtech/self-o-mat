//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_EXPOSURECORRECTION_CONTROLLER_H
#define SELF_O_MAT_EXPOSURECORRECTION_CONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ExposureCorrectionController : public ChoiceRangeController {
            public:
                ExposureCorrectionController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                        ChoiceRangeController(gp, camera, rootWidget, {"exposurecompensation"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_ExposureCorrectionController_H
