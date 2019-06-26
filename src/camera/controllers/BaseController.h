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

                bool dirty = false;

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
                 * Gets the current choice for a widget
                 * @param widget the widget
                 * @param choices the possible choices
                 * @return -1 on error else the index
                 */
                int getProperty(CameraWidget *widget, std::vector<std::string> &choices);

                /**
                 * Gets the current choice for a text widget
                 * @param widget the widget
                 * @param choices the possible choices
                 * @return the property. N/A on error.
                 */
                std::string getPropertyText(CameraWidget *widget);


                /**
                 * Set the value of the widget. Range and Radio widgets are supported
                 * @param widget the widget
                 * @param choices the choices
                 * @param choice the index of the choice
                 * @return true on success
                 */
                bool setProperty(CameraWidget *widget, std::vector<std::string> &choices, int choice);

                /**
                 * Sets the property of a toggle widget.
                 *
                 * @param widget the widget
                 * @param value the value
                 */
                bool setPropertyToggle(CameraWidget *widget, bool value);

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

                virtual bool pullSettings() = 0;

                bool isDirty();

                void resetDirty();
            };
        }
    }
}

#endif //SELF_O_MAT_IGPHOTOCONTROLLER_H
