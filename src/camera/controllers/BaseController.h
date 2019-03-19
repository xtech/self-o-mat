//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_IGPHOTOCONTROLLER_H
#define SELF_O_MAT_IGPHOTOCONTROLLER_H

#include <string>
#include <vector>
#include <tools/verbose.h>

extern "C" {
#include <gphoto2/gphoto2.h>
}

namespace selfomat {
    namespace camera {
        namespace gphoto {

            class BaseController {
            private:
                static const std::string TAG;

            protected:
                GPContext *gp;
                Camera *camera;
                CameraWidget *rootWidget;

                /**
                 * Finds a camera widget
                 * @param widgetName: The name
                 * @param target: The target where to store the widget
                 * @return true, if found. false otherwise.
                 */
                bool findWidget(std::string widgetName, CameraWidget **target);

                /**
                 * Loads the choices a user has for a given widget
                 * @param widget the widget
                 * @param choices the vector where choices will be stored
                 * @return true, if successful
                 */
                bool loadChoices(CameraWidget *widget, std::vector<std::string> &choices);

                /**
                 * Gets the current value of a given widget
                 * @param widget the widget
                 * @return the value.
                 */
                std::string getProperty(CameraWidget *widget);

                /**
                 * Updates the widget to a specific value
                 * @param widget the widget
                 * @param value the value
                 * @return true on success
                 */
                bool setProperty(CameraWidget *widget, std::string value);

            public:
                BaseController(GPContext *gp, Camera *camera, CameraWidget *rootWidget) : gp(gp), camera(camera),
                                                                                                rootWidget(
                                                                                                        rootWidget) {}

                virtual ~BaseController() {
                    // We do not free the camera here as we only use it.
                }

                /**
                 * @return true, if camera is supported by this controller
                 */
                virtual bool supportsCamera() = 0;
            };
        }
    }
}

#endif //SELF_O_MAT_IGPHOTOCONTROLLER_H
