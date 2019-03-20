//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_WHITE_BALANCE_CONTROLLER_H
#define SELF_O_MAT_WHITE_BALANCE_CONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class WhiteBalanceController : public ChoiceRangeController {
            public:
                WhiteBalanceController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                    ChoiceRangeController(gp, camera, rootWidget, {"iso"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_WhiteBalanceController_H
