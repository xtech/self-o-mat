//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_BOOTHGUI_H
#define SELF_O_MAT_BOOTHGUI_H


#include <cstdint>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <boost/thread.hpp>
#include <iostream>
#include <queue>
#include "FPSCounter.h"
#include "tools/ILogger.h"
#include "IGui.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <codecvt>
#include <tools/readfile.h>
#include <logic/ILogicController.h>
#include <tools/verbose.h>

#define DEBUG_QUEUE_SIZE 100

#define COLOR_MAIN          sf::Color(20, 64, 66, 255)
#define COLOR_MAIN_LIGHT    sf::Color(155, 194, 189)
#define COLOR_ALERT         sf::Color(200, 0, 0)

namespace selfomat {
    namespace ui {

        struct Alert {
            sf::Int32 startTime;
            sf::Int32 endTime;
            std::wstring text;
            bool hint;
        };

        class BoothGui : public IGui {
        public:
            explicit BoothGui(bool debug, logic::ILogicController *logicController);

        private:

            static std::string TAG;

            logic::ILogicController *logicController;

            bool isRunning;

            bool debug;

            bool printerEnabled;
            bool templateEnabled;

            bool shouldShowAgreement;

            GUI_STATE currentState;
            sf::Clock stateTimer;

            boost::mutex alertMutex;
            std::map<ALERT_TYPE, Alert> alerts;
            sf::Clock alertTimer;

            sf::VideoMode videoMode;
            sf::RenderWindow window;
            sf::Font hackFont;
            sf::Font iconFont;
            sf::Font mainFont;
            sf::Color clearColor;
            sf::Text debugText;
            sf::Text iconText;
            sf::Text alertText;
            sf::Text printText;
            sf::Text agreementText;
            sf::Texture imageTexture;
            sf::Sprite imageSprite;
            sf::Sprite finalImageSprite;
            sf::Mutex imageMutex;
            sf::RectangleShape rect_overlay;

            sf::CircleShape count_down_circle;

            sf::Mutex debugLogQueueMutex;
            std::deque<std::string> debugLogQueue;

            // Live frame
            sf::Texture textureLiveOverlay;
            sf::Texture texturePrintOverlay;
            sf::Texture textureFinalImageOverlay;
            sf::Sprite imageSpriteLiveOverlay;
            sf::Sprite imageSpritePrintOverlay;

            bool templateLoaded = false;
            sf::Sprite imageSpriteFinalOverlay;

            sf::Texture textureNoCamera;
            sf::Sprite imageNoCamera;

            std::wstring agreement;

            int finalOverlayOffsetX, finalOverlayOffsetY, finalOverlayOffsetW, finalOverlayOffsetH;


            void *imageBuffer = nullptr;
            uint32_t imageWidth = 0;
            uint32_t imageHeight = 0;
            bool imageDirty = true;
            bool imageShown = false;

            boost::thread renderThreadHandle;

            FPSCounter renderFrameCounter;
            FPSCounter cameraFrameCounter;

            boost::mutex guiStateMutex;

            void renderThread();

            void setState(GUI_STATE newState);

            float easeOutSin(float t, float b, float c, float d);

            void drawPrintOverlay(float percentage = 1.0f);
            void drawAlerts();
            void drawAgreement(float alpha = 1);
            void drawDebug();

            void removeAlert(ALERT_TYPE type, bool forced);

        public:
            BoothGui();


            const GUI_STATE getCurrentGuiState() override {
                boost::unique_lock<boost::mutex> lk(guiStateMutex);
                return currentState;
            }

            void setLogicController(logic::ILogicController *logicController) {
                this->logicController = logicController;
            }

        protected:
            void log(std::string s) override;

        public:

            void setDebugOutput(bool debug);

            bool start() override;

            void stop() override;

            void initialized() override;

            void reloadTemplate() override;

            void updatePreviewImage(void *data, uint32_t width, uint32_t height) override;

            void hidePreviewImage() override {
                imageMutex.lock();
                imageShown = false;
                setState(STATE_BLACK);
                imageMutex.unlock();
            }

            void notifyFinalImageSent() override {
                setState(STATE_TRANS_BLACK_FINAL);
            }

            void notifyPreviewIncoming() override {
                if(currentState == STATE_BLACK) {
                    // We're before the final image but we get a new preview - there seems to be an error.
                    // Go fade to live
                    setState(STATE_TRANS_PREV2_PREV3);
                } else {
                    setState(STATE_TRANS_PRINT_PREV1);
                }
            }

            void showAgreement() override;
            void hideAgreement() override;

            bool hasAlert(ALERT_TYPE type) override;
            void addAlert(ALERT_TYPE type, std::wstring text, bool autoRemove = false, bool isHint = false) override;
            void removeAlert(ALERT_TYPE type) override;

            void setPrinterEnabled(bool printerEnabled) override;
            void setTemplateEnabled(bool templateEnabled) override;

            void cancelPrint() override;

            ~BoothGui() override;


        };
    }
}

#endif //SELF_O_MAT_BOOTHGUI_H
