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

            enum GUI_STATE {
                STATE_INIT,
                STATE_AGREEMENT,
                STATE_TRANS_AGREEMENT,
                STATE_LIVE_PREVIEW,
                STATE_BLACK,
                STATE_TRANS_BLACK_FINAL,
                STATE_FINAL_IMAGE,
                STATE_TRANS_FINAL_IMAGE_PRINT,
                STATE_FINAL_IMAGE_PRINT,
                STATE_TRANS_PRINT_PREV1,
                STATE_TRANS_PREV1_PREV2,
                STATE_TRANS_PREV2_PREV3
            };

            virtual const GUI_STATE getCurrentGuiState() = 0;

            virtual void initialized() = 0;

            virtual bool start() = 0;

            virtual void stop() = 0;

            virtual void updatePreviewImage(void *data, uint32_t width, uint32_t height) = 0;

            virtual void hidePreviewImage() = 0;

            virtual void notifyFinalImageSent() = 0;

            virtual void notifyPreviewIncoming() = 0;

            virtual void addAlert(std::string icon, std::wstring text, bool autoRemove = false) = 0;

            virtual void removeAlert(std::string icon) = 0;

<<<<<<< HEAD
            virtual bool isWaitingForButton() = 0;

            virtual void buttonPushed() = 0;

=======
            virtual void setPrinterEnabled(bool printerEnabled) = 0;

            virtual void setTemplateEnabled(bool templateEnabled) = 0;
>>>>>>> master
        };
    }
}

#endif //SELF_O_MAT_IGUI_H
