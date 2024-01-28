//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_IGUI_H
#define SELF_O_MAT_IGUI_H

#include "tools/ILogger.h"

#include <boost/unordered_map.hpp>
#include <boost/assign/list_of.hpp>

using namespace selfomat::tools;

namespace selfomat {
    namespace ui {

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
            STATE_FINAL_IMAGE_PRINT_CANCELED,
            STATE_FINAL_IMAGE_PRINT_CONFIRMED,
            STATE_TRANS_PRINT_PREV1,
            STATE_TRANS_PREV1_PREV2,
            STATE_TRANS_PREV2_PREV3
        };

        enum ALERT_TYPE {
            ALERT_CAMERA,
            ALERT_PRINTER,
            ALERT_PRINTER_HINT,
            ALERT_STORAGE,
            ALERT_STORAGE_ERROR,
            ALERT_CAMERA_HINT,
            ALERT_INFO,
            ALERT_WARNING,
            ALERT_TEMPLATE
        };

        class IGui : public ILogger {
        public:

            virtual const GUI_STATE getCurrentGuiState() = 0;

            virtual void initialized() = 0;

            virtual bool start() = 0;

            virtual void stop() = 0;

            virtual void reloadTemplate() = 0;

            virtual void updatePreviewImage(void *data, uint32_t width, uint32_t height) = 0;

            virtual void hidePreviewImage() = 0;

            virtual void notifyFinalImageSent() = 0;

            virtual void notifyPreviewIncoming() = 0;

            virtual void updateCountdown(int new_countdown) = 0;

            virtual bool hasAlert(ALERT_TYPE type) = 0;
            virtual void addAlert(ALERT_TYPE type, std::wstring text, bool autoRemove = false, bool isHint = false) = 0;
            virtual void removeAlert(ALERT_TYPE type) = 0;

            virtual void showAgreement() = 0;
            virtual void hideAgreement() = 0;

            virtual void setPrinterEnabled(bool printerEnabled) = 0;

            virtual void setTemplateEnabled(bool templateEnabled) = 0;

            virtual void cancelPrint() = 0;

            virtual void confirmPrint() = 0;

            virtual void setDebugOutput(bool debug) = 0;

            const boost::unordered_map<ALERT_TYPE,const char*> alertTypeToString = boost::assign::map_list_of
                    (ALERT_CAMERA, "C")
                    (ALERT_PRINTER, "P")
                    (ALERT_PRINTER_HINT, "P")
                    (ALERT_STORAGE, "U")
                    (ALERT_STORAGE_ERROR, "U")
                    (ALERT_CAMERA_HINT, "C")
                    (ALERT_INFO, "I")
                    (ALERT_WARNING, "A")
                    (ALERT_TEMPLATE, "T");

        };
    }
}

#endif //SELF_O_MAT_IGUI_H
