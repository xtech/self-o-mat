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

#define DEBUG_QUEUE_SIZE 100
#define DEBUG_LEVEL_DEBUG 1
#define DEBUG_LEVEL_INFO 2
#define DEBUG_LEVEL_WARN 3
#define DEBUG_LEVEL_ERROR 4

#define COLOR_MAIN          sf::Color(20, 64, 66, 255)
#define COLOR_MAIN_LIGHT    sf::Color(155, 194, 189)
#define COLOR_ALERT         sf::Color(200, 0, 0)

namespace selfomat {
    namespace ui {

        struct Alert {
            sf::Int32 startTime;
            sf::Int32 endTime;
            std::wstring text;
        };

        class BoothGui : public IGui {
        private:
            bool isRunning;

            enum GUI_STATE {
                STATE_INIT,
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

            GUI_STATE currentState;
            sf::Clock stateTimer;

            boost::recursive_mutex alertMutex;
            std::map<std::string, Alert> alerts;
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
            sf::Sprite imageSpriteFinalOverlay;

            int finalOverlayOffsetTop;
            int finalOverlayOffsetLeft;
            int finalOverlayOffsetBottom;
            int finalOverlayOffsetRight;


            void *imageBuffer = nullptr;
            uint32_t imageWidth = 0;
            uint32_t imageHeight = 0;
            bool imageDirty = true;
            bool imageShown = false;

            boost::thread renderThreadHandle;

            FPSCounter renderFrameCounter;
            FPSCounter cameraFrameCounter;

            void renderThread();

            void setState(GUI_STATE newState);

            float easeOutSin(float t, float b, float c, float d);

            void drawAlerts();
            void drawDebug();

            void removeAlert(std::string icon, bool forced);

        public:
            BoothGui();


            void log(int level, std::string s);

            void log(int level, const char *s) {
                log(level, std::string(s));
            }

            void logDebug(std::string s) override { log(DEBUG_LEVEL_DEBUG, s); }

            void logDebug(const char *s) override { log(DEBUG_LEVEL_DEBUG, s); }

            void logInfo(std::string s) override { log(DEBUG_LEVEL_INFO, s); }

            void logInfo(const char *s) override { log(DEBUG_LEVEL_INFO, s); }

            void logWarn(std::string s) override { log(DEBUG_LEVEL_WARN, s); }

            void logWarn(const char *s) override { log(DEBUG_LEVEL_WARN, s); }

            void logError(std::string s) override { log(DEBUG_LEVEL_ERROR, s); }

            void logError(const char *s) override { log(DEBUG_LEVEL_ERROR, s); }


            bool start() override;

            void stop() override;

            void initialized() override;

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
                setState(STATE_TRANS_PRINT_PREV1);
            }

            void addAlert(std::string icon, std::wstring text, bool autoRemove = false);
            void removeAlert(std::string icon);

            ~BoothGui() override;

        };
    }
}

#endif //SELF_O_MAT_BOOTHGUI_H
