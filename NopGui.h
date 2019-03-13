//
// Created by clemens on 12.03.19.
//

#ifndef SELF_O_MAT_NOPGUI_H
#define SELF_O_MAT_NOPGUI_H

#include "IGui.h"
#include <iostream>

class NopGui : public IGui {
public:
    void initialized() override;

    bool start() override;

    void stop() override;

    void updatePreviewImage(void *data, uint32_t width, uint32_t height) override;

    void hidePreviewImage() override;

    void notifyFinalImageSent() override;

    void notifyPreviewIncoming() override;

    void logDebug(std::string s) override;

    void logDebug(const char *s) override;

    void logInfo(std::string s) override;

    void logInfo(const char *s) override;

    void logWarn(std::string s) override;

    void logWarn(const char *s) override;

    void logError(std::string s) override;

    void logError(const char *s) override;
};


#endif //SELF_O_MAT_NOPGUI_H
