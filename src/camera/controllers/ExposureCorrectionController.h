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
            private:
                int normalChoice;
                int preTriggerChoice;
                bool isTriggered;
            public:
                ExposureCorrectionController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) :
                        ChoiceRangeController(gp, camera, rootWidget, {"exposurecompensation"}) {
                    normalChoice = -1;
                    preTriggerChoice = -1;
                    isTriggered = false;
                }

                bool setChoice(int choice) override {
                    normalChoice = choice;

                    if(isTriggered)
                        return true;

                    return ChoiceRangeController::setChoice(choice);
                }

                int getChoice() override {
                    return normalChoice;
                }

                bool pullSettings() override {
                    bool success = ChoiceRangeController::pullSettings();
                    if(success && (normalChoice < 0 || preTriggerChoice < 0)) {
                        // we have pulled the choices from the camera and not yet initialized the duplicated
                        normalChoice = preTriggerChoice = getChoice();
                    }
                    return success;
                }

                bool setTriggerChoice(int choice) {
                    preTriggerChoice = choice;
                }

                int getTriggerChoice() {
                    return preTriggerChoice;
                }

                bool preTrigger() {
                    return ChoiceRangeController::setChoice(preTriggerChoice);
                }

                bool postTrigger() {
                    return ChoiceRangeController::setChoice(normalChoice);
                }
            };
        }
    }
}



#endif //SELF_O_MAT_ExposureCorrectionController_H
