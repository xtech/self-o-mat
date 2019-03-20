//
// Created by clemens on 19.03.19.
//

#ifndef SELF_O_MAT_IMAGE_FORMAT_CONTROLLER_H
#define SELF_O_MAT_IMAGE_FORMAT_CONTROLLER_H


#include "ChoiceRangeController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class ImageFormatController : public ChoiceRangeController {
            public:
                ImageFormatController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                    ChoiceRangeController(gp, camera, rootWidget, {"imageformat"}) {
                }
            };
        }
    }
}



#endif //SELF_O_MAT_ImageFormatController_H
