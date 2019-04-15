//
// Created by clemens on 12.03.19.
//

#include "NopGui.h"

using namespace selfomat::ui;

void NopGui::initialized() {
    std::cout << "GUI Initialized" << std::endl;
}

bool NopGui::start() {
    std::cout << "GUI started" << std::endl;
    return true;
}

void NopGui::stop() {
    std::cout << "GUI stopped" << std::endl;
}

void NopGui::updatePreviewImage(void *data, uint32_t width, uint32_t height) {
    //std::cout << "new preview image with size " << width << "x" << height << std::endl;
}

void NopGui::hidePreviewImage() {
    std::cout << "hide preview image callback" << std::endl;
}

void NopGui::notifyFinalImageSent() {
    std::cout << "final image callback" << std::endl;
}

void NopGui::notifyPreviewIncoming() {
    std::cout << "preview incoming callback" << std::endl;
}

void NopGui::logDebug(std::string s) {
    std::cout << "debug: " << s << std::endl;
}

void NopGui::logDebug(const char *s) {
    std::cout << "debug: " << s << std::endl;
}

void NopGui::logInfo(std::string s) {
    std::cout << "info: " << s << std::endl;
}

void NopGui::logInfo(const char *s) {
    std::cout << "info: " << s << std::endl;
}

void NopGui::logWarn(std::string s) {
    std::cout << "warn: " << s << std::endl;
}

void NopGui::logWarn(const char *s) {
    std::cout << "warn: " << s << std::endl;
}

void NopGui::logError(std::string s) {
    std::cout << "error: " << s << std::endl;
}

void NopGui::logError(const char *s) {
    std::cout << "error: " << s << std::endl;
}

void NopGui::addAlert(ALERT_TYPE type, std::wstring text, bool autoRemove) {

}

void NopGui::removeAlert(ALERT_TYPE type) {

}

void NopGui::setPrinterEnabled(bool printerEnabled) {

}

void NopGui::setTemplateEnabled(bool templateEnabled) {

}
