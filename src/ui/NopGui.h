//
// Created by clemens on 12.03.19.
//

#ifndef SELF_O_MAT_NOPGUI_H
#define SELF_O_MAT_NOPGUI_H

#include "IGui.h"
#include <iostream>

namespace selfomat {
    namespace ui {
        class NopGui : public IGui {
        public:
            void initialized() override;

            bool start() override;

            void stop() override;

            void updatePreviewImage(void *data, uint32_t width, uint32_t height) override;

            void hidePreviewImage() override;

            void notifyFinalImageSent() override;

            void notifyPreviewIncoming() override;


            void addAlert(ALERT_TYPE type, std::wstring text, bool autoRemove, bool isHint) override;

            void removeAlert(ALERT_TYPE type) override;

            void setPrinterEnabled(bool printerEnabled) override;

            void setTemplateEnabled(bool templateEnabled) override;

            const GUI_STATE getCurrentGuiState() override;

        protected:
            void log(std::string s) override;
        };
    }
}

#endif //SELF_O_MAT_NOPGUI_H
