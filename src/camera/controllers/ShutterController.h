//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_SHUTTER_SPEED_CONTROLLER_H
#define SELF_O_MAT_SHUTTER_SPEED_CONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ShutterSpeedController : public ChoiceRangeController {
            public:
                ShutterSpeedController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                    ChoiceRangeController(gp, camera, rootWidget, {"shutterspeed"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_ShutterSpeedController_H
