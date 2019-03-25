//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_APERTURECONTROLLER_H
#define SELF_O_MAT_APERTURECONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ApertureController : public ChoiceRangeController {
            public:
                ApertureController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                    ChoiceRangeController(gp, camera, rootWidget, {"aperture", "f-number"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_APERTURECONTROLLER_H
