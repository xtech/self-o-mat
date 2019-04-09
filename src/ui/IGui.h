//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_IGUI_H
#define SELF_O_MAT_IGUI_H

#include "tools/ILogger.h"

using namespace selfomat::tools;

namespace selfomat {
    namespace ui {
        class IGui : public ILogger {
        public:

            virtual void initialized() = 0;

            virtual bool start() = 0;

            virtual void stop() = 0;

            virtual void updatePreviewImage(void *data, uint32_t width, uint32_t height) = 0;

            virtual void hidePreviewImage() = 0;

            virtual void notifyFinalImageSent() = 0;

            virtual void notifyPreviewIncoming() = 0;

            virtual void addAlert(std::string icon, std::wstring text, bool autoRemove = false) = 0;

            virtual void removeAlert(std::string icon) = 0;

        };
    }
}

#endif //SELF_O_MAT_IGUI_H
