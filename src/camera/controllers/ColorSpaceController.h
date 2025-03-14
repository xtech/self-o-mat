//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_COLORSPACE_CONTROLLER_H
#define SELF_O_MAT_COLORSPACE_CONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ColorSpaceController : public ChoiceRangeController {
            public:
                ColorSpaceController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                    ChoiceRangeController(gp, camera, rootWidget, {"colorspace"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_ColorSpaceController_H
