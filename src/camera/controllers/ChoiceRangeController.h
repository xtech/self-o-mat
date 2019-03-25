//
// Created by clemens on 20.03.19.
//

#ifndef SELF_O_MAT_CHOICERANGECONTROLLER_H
#define SELF_O_MAT_CHOICERANGECONTROLLER_H


#include "BaseController.h"
#include <vector>

namespace selfomat {
    namespace camera {
        namespace gphoto {

            /**
             * Subclass this controller to implement simple choice or range based camera settings.
             * E.g. Aperture, ISO, ...
             */
            class ChoiceRangeController : public BaseController {
            private:
                static const std::string TAG;
                CameraWidget *p_choiceRangeWidget = nullptr;
                int currentChoice;
                std::vector<std::string> choices;


            public:
                ChoiceRangeController(GPContext *gp, Camera *camera, CameraWidget *rootWidget, std::vector<std::string> widgetNames);

                bool supportsCamera() override;

                bool pullSettings() override;

                virtual bool setChoice(int choice);
                virtual int getChoice();
                virtual const std::vector<std::string> * const getChoices();

                virtual bool isReadOnly();
            };
        }
    }
}


#endif //SELF_O_MAT_CHOICERANGECONTROLLER_H
