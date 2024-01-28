//
// Created by clemens on 21.01.19.
//

#include "BoothGui.h"

using namespace std;
using namespace selfomat::ui;

std::string BoothGui::TAG = "BOOTH_GUI";

BoothGui::BoothGui(bool fullscreen, bool debug, logic::ILogicController *logicController) : debugLogQueue(), stateTimer(), alertTimer() {
    // TODO: fixed resolution -> variable resolution
    videoMode = sf::VideoMode(1280, 800);
    this->currentState = STATE_INIT;
    this->shouldShowAgreement = false;
    this->debug = debug;
    this->logicController = logicController;
    this->fullscreen = fullscreen;
}

BoothGui::~BoothGui() {
}

bool BoothGui::start() {

    FilesystemLogger::INSTANCE.registerDelegate(this);
    // Load assets
    if (!hackFont.loadFromFile("./assets/Hack-Regular.ttf")) {
        LOG_E(TAG, "Could not load font.");
        return false;
    }

    if (!iconFont.loadFromFile("./assets/self-o-mat.ttf")) {
        LOG_E(TAG, "Could not load font.");
        return false;
    }

    if (!mainFont.loadFromFile("./assets/AlegreyaSans-Bold.ttf")) {
        LOG_E(TAG, "Could not load font.");
        return false;
    }

    if (!textureLiveOverlay.loadFromFile("./assets/live.png")) {
        LOG_E(TAG, "Could not load live asset.");
        return false;
    }
    imageSpriteLiveOverlay.setTexture(textureLiveOverlay);

    if (!texturePrintOverlay.loadFromFile("./assets/print_overlay.png")) {
        LOG_E(TAG, "Could not print overlay asset.");
        return false;
    }
    imageSpritePrintOverlay.setTexture(texturePrintOverlay);

    if (!textureNoCamera.loadFromFile("./assets/no-camera.png")) {
        LOG_E(TAG, "Could not load no camera asset.");
        return false;
    }
    imageNoCamera.setTexture(textureNoCamera);

    readFile("./assets/agreement.txt", agreement);
    if (agreement.length() < 1) {
        LOG_E(TAG, "Could not load agreement text.");
        return false;
    }

    reloadTemplate();

    rect_overlay = sf::RectangleShape(sf::Vector2f(videoMode.width, videoMode.height));
    count_down_circle = sf::CircleShape(19.0f);

    isRunning = true;
    renderThreadHandle = boost::thread(boost::bind(&BoothGui::renderThread, this));
    return true;
}

void BoothGui::stop() {
    isRunning = false;
    if (renderThreadHandle.joinable()) {
        LOG_I(TAG, "Waiting for gui to stop");
        renderThreadHandle.join();
    }
}

void BoothGui::reloadTemplate() {

    imageMutex.lock();

    templateLoaded = textureFinalImageOverlay.loadFromFile(std::string(getenv("HOME")) + "/.template_screen.png");
    if (!templateLoaded) {
        LOG_E(TAG, "Could not load screen template asset.");
    } else {
        imageSpriteFinalOverlay.setTexture(textureFinalImageOverlay);
    }

    if(templateLoaded) {
        // Read properties for the template
        boost::property_tree::ptree ptree;
        try {
            boost::property_tree::read_json(std::string(getenv("HOME")) + "/.template_screen_props.json", ptree);
            finalOverlayOffsetX = ptree.get<int>("offset_x");
            finalOverlayOffsetY = ptree.get<int>("offset_y");
            finalOverlayOffsetW = ptree.get<int>("offset_w");
            finalOverlayOffsetH = ptree.get<int>("offset_h");
        } catch (boost::exception &e) {
            LOG_E(TAG, std::string("Error loading template properties: ") + boost::diagnostic_information(e));
        }
    }

    imageMutex.unlock();
}

void BoothGui::updatePreviewImage(void *data, uint32_t width, uint32_t height) {
    if (getCurrentGuiState() == STATE_TRANS_PRINT_PREV1)
        return;
    imageMutex.lock();
    imageBuffer = data;
    imageWidth = width;
    imageHeight = height;
    imageDirty = true;
    imageShown = true;
    imageMutex.unlock();

    if(getCurrentGuiState() == STATE_TRANS_PREV1_PREV2) {
        // We now have a preview image, let it continue
        setState(STATE_TRANS_PREV2_PREV3);
    }
}


void BoothGui::renderThread() {
    LOG_I(TAG, "Render thread started!");
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    settings.majorVersion = 2;
    settings.minorVersion = 1;
    auto style = fullscreen ? sf::Style::Fullscreen : sf::Style::Default;

    window.create(videoMode, "self-o-mat", style, settings);

    window.setVerticalSyncEnabled(true);

    window.setFramerateLimit(60);


    // Init variables
    clearColor = sf::Color(0, 0, 0);
    debugText.setFont(hackFont);
    debugText.setFillColor(sf::Color::Black);
    debugText.setOutlineColor(sf::Color::White);
    debugText.setOutlineThickness(1.5);
    debugText.setCharacterSize(15);

    iconText.setFont(iconFont);
    iconText.setFillColor(COLOR_ALERT);
    iconText.setCharacterSize(50);

    countdownText.setFont(hackFont);
    countdownText.setFillColor(COLOR_COUNTDOWN);
    countdownText.setCharacterSize(400);
    countdownText.setStyle(1); // Bold

    alertText.setFont(mainFont);
    alertText.setFillColor(COLOR_ALERT);
    alertText.setCharacterSize(50);
    alertText.setStyle(1); //Bold

    printText.setFont(mainFont);
    printText.setFillColor(COLOR_MAIN);
    printText.setCharacterSize(80);
    printText.setStyle(1);

    agreementText.setFont(hackFont);
    agreementText.setFillColor(sf::Color::White);
    agreementText.setStyle(1); //Bold

    imageTexture.create(videoMode.width, videoMode.height);
    imageSprite = sf::Sprite(imageTexture);
    //finalImageSprite = sf::Sprite(imageTexture);

    window.setActive(true);
    sf::Event event{};
    stateTimer.restart();
    while (isRunning) {

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if(logicController != nullptr && event.type == sf::Event::MouseButtonPressed) {
                if(event.mouseButton.button == sf::Mouse::Button::Left) {
                    logicController->trigger();
                }
            }
        }


        window.clear(clearColor);

        // If we have new image data, update the texture
        renderFrameCounter.nextFrame();
        if (imageDirty) {
            cameraFrameCounter.nextFrame();
            imageMutex.lock();
            if (imageWidth > 0 && imageHeight > 0) {
                auto textureSize = imageTexture.getSize();
                if (textureSize.x != imageWidth || textureSize.y != imageHeight) {
                    // Recreate the texture if needed
                    if (imageWidth > textureSize.x || imageHeight > textureSize.y) {
                        LOG_D(TAG, "Recreating texture to fit the new image");
                        imageTexture.create(imageWidth, imageHeight);
                    }
                }



                // Calculate the sprite's new shape so that the image fits
                float scaleX = (float) window.getSize().x / (float) imageWidth;
                float scaleY = (float) window.getSize().y / (float) imageHeight;
                float scale = max(scaleX, scaleY);

                imageSprite.setScale(-scale, scale);

                float windowCenterY = window.getSize().y / 2.0f;
                float windowCenterX = window.getSize().x / 2.0f;

                float imageCenterX = (float) imageWidth * scale * 0.5f;
                float imageCenterY = (float) imageHeight * scale * 0.5f;


                imageSprite.setPosition(videoMode.width - (windowCenterX - imageCenterX),
                                        windowCenterY - imageCenterY);


                if(templateEnabled && templateLoaded) {
                    float finalOverlayCenterX =
                            (float) finalOverlayOffsetX + (float) finalOverlayOffsetW / 2.0f;
                    float finalOverlayCenterY =
                            (float) finalOverlayOffsetY + (float) finalOverlayOffsetH / 2.0f;


                    finalImageSprite.setTexture(imageTexture, true);
                    finalImageSprite.setTextureRect(sf::IntRect(0, 0, imageWidth, imageHeight));

                    // Same for the final sprite
                    float finalScaleX = (float) finalOverlayOffsetW / (float) imageWidth;
                    float finalScaleY =
                            (float) finalOverlayOffsetH / (float) imageHeight;
                    float finalScale = max(finalScaleX, finalScaleY);
                    finalImageSprite.setScale(finalScale, finalScale);

                    float finalImageCenterX = (float) imageWidth * finalScale * 0.5f;
                    float finalImageCenterY = (float) imageHeight * finalScale * 0.5f;

                    finalImageSprite.setPosition(finalOverlayCenterX - finalImageCenterX,
                                                 finalOverlayCenterY - finalImageCenterY);

                } else {
                    finalImageSprite.setTexture(imageTexture, true);
                    finalImageSprite.setTextureRect(sf::IntRect(0, 0, imageWidth, imageHeight));
                    finalImageSprite.setScale(scale, scale);
                    finalImageSprite.setPosition(windowCenterX - imageCenterX,
                                            windowCenterY - imageCenterY);
                }

                imageTexture.update((sf::Uint8 *) imageBuffer, imageWidth, imageHeight, 0, 0);
                imageDirty = false;
            }
            imageMutex.unlock();
        }


        switch (getCurrentGuiState()) {
            case STATE_LIVE_PREVIEW: {

                auto alert = alerts.find(ALERT_CAMERA);
                if (alert != alerts.end()) {

                    window.draw(imageNoCamera);

                } else if (imageShown) {
                    // We can show a preview image
                    window.draw(imageSprite);

                    float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                    float alpha_float = min(1.0f, timeInState / 300.0f);
                    float alpha_float_cos = 0.4f * cos(timeInState / 800.0f) + .6f;
                    auto alpha = uint8_t(alpha_float * alpha_float_cos * 255.0f);

                    imageSpriteLiveOverlay.setColor(sf::Color(255, 255, 255, alpha));
                    window.draw(imageSpriteLiveOverlay);
                }
            }
                break;
            case STATE_BLACK:
                break;
            case STATE_TRANS_BLACK_FINAL: {
                // Fade in
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float alpha_float = min(1.0f, timeInState / 300.0f);
                auto alpha = uint8_t((1.0f - alpha_float) * 255.0f);

                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }

                rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
                window.draw(rect_overlay);

                // Switch state as soon as the animation is over
                if (timeInState > 300.0f) {
                    setState(STATE_FINAL_IMAGE);
                }
            }
                break;
            case STATE_FINAL_IMAGE: {
                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }

                float duration = 1000.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                if (timeInState >= duration) {
                    setState(STATE_TRANS_FINAL_IMAGE_PRINT);
                }
            }
                break;
            case STATE_TRANS_FINAL_IMAGE_PRINT: {

                float duration = 350.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float linearPercentage = min(1.0f, timeInState / duration);

                // Add easing to the percentage
                float percentage = easeOutSin(linearPercentage, 0.0f, 1.0f, 1.0f);

                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }


                drawPrintOverlay(percentage);

                if (timeInState >= duration) {
                    setState(STATE_FINAL_IMAGE_PRINT);
                }
            }
                break;
            case STATE_FINAL_IMAGE_PRINT: {
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();

                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }

                drawPrintOverlay();

                if (timeInState >= 2500) {
                    setState(STATE_FINAL_IMAGE_PRINT_CANCELED);
                }
            }
                break;
            case STATE_FINAL_IMAGE_PRINT_CONFIRMED: /* intentional fall-through */
            case STATE_FINAL_IMAGE_PRINT_CANCELED: {
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float duration = 200.0f;
                float percentage = 1 - min(1.0f, timeInState / duration);

                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }

                drawPrintOverlay(percentage);
            }
                break;
            case STATE_TRANS_PRINT_PREV1: {
                float duration = 250.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float linearPercentage = timeInState / duration;

                float alpha = max(0.0f, min(255.0f, linearPercentage * 255.0f));

                window.draw(finalImageSprite);
                if(templateEnabled && templateLoaded) {
                    window.draw(imageSpriteFinalOverlay);
                }

                rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
                window.draw(rect_overlay);

                if (timeInState >= duration && imageShown) {
                    imageShown = false;
                    setState(STATE_TRANS_PREV1_PREV2);
                }
            }
            break;

            case STATE_TRANS_PREV1_PREV2:
                // Nop, we want black
                break;
            case STATE_TRANS_PREV2_PREV3: {
                // Fade in
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float alpha_float = min(1.0f, timeInState / 300.0f);
                auto alpha = uint8_t((1.0f - alpha_float) * 255.0f);
                window.draw(imageSprite);
                //window.draw(finalImageSprite);
                rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
                window.draw(rect_overlay);

                // Switch state as soon as the animation is over
                if (timeInState > 300.0f) {
                    setState(STATE_LIVE_PREVIEW);
                }

            }
                break;
            case STATE_AGREEMENT: {
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float alpha = min(1.0f, timeInState / 300.0f);
                drawAgreement(alpha);
            }
                break;
            case STATE_TRANS_AGREEMENT: {
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float alpha = min(1.0f, timeInState / 300.0f);
                drawAgreement(1 - alpha);
                if (timeInState > 300.0f) {
                    setState(STATE_TRANS_PREV2_PREV3);
                }
            }
                break;
            case STATE_INIT:
                if (debug)
                    window.clear(sf::Color(0, 255, 0));
                break;
            default:
                if (debug)
                    window.clear(sf::Color(255, 0, 0));
                break;
        }

        drawCountdown();
        drawAlerts();
        drawDebug();

        // Draw the window
        window.display();
    }
    window.close();
}

void BoothGui::setState(GUI_STATE newState) {
    boost::unique_lock<boost::mutex> lk(guiStateMutex);

    currentState = newState;
    stateTimer.restart();
}

void BoothGui::initialized() {
    // Move the state from initialized to preview live
    if (shouldShowAgreement) {
        setState(STATE_AGREEMENT);
        shouldShowAgreement = false;
    } else {
        setState(STATE_TRANS_PREV2_PREV3);
    }
}



void BoothGui::drawPrintOverlay(float percentage) {
    if(!printerEnabled)
        return;
    float timeInState = stateTimer.getElapsedTime().asMilliseconds();
    float templateY = window.getSize().y - abs(percentage) * texturePrintOverlay.getSize().y;

    sf::RectangleShape printBackground(sf::Vector2f(window.getSize().x, texturePrintOverlay.getSize().y));
    printBackground.setFillColor(sf::Color::White);
    printBackground.setPosition(0, templateY);
    window.draw(printBackground);

    if( logicController != nullptr ) {
      if( logicController->getPrintConfirmationEnabled() ) {
        printText.setString(logicController->getTranslation("frontend.confirm_print"));
      }
      else {
        printText.setString(logicController->getTranslation("frontend.cancel_print"));
      }
    }
    sf::FloatRect textRect = printText.getLocalBounds();
    printText.setPosition((window.getSize().x - textRect.width) / 2.0f, templateY + 10);
    window.draw(printText);

    imageSpritePrintOverlay.setPosition(((window.getSize().x + max(textRect.width, 600.0f))/ 2.0f) + 20, templateY);
    window.draw(imageSpritePrintOverlay);


    for(int i = 0; i < 6; i++) {
        count_down_circle.setPosition(340.0f + i*(113.0f),templateY + 149.0f - 19.0f);
        if(percentage == -1 || (percentage >= 1 && timeInState >= 500.0 * i)) {
            count_down_circle.setFillColor(COLOR_MAIN);
        } else {
            count_down_circle.setFillColor(COLOR_MAIN_LIGHT);
        }

        window.draw(count_down_circle);
    }

}

void BoothGui::drawAgreement(float alpha) {
    std::wstring title = L"MIT Software License";
    std::wstring button = L"Accept?";
    std::vector<std::wstring> blocks;
    u_int8_t lines = 0;
    u_int8_t margin = 50;
    u_int8_t marginTop = 175;
    u_int8_t lineSpacing = 40;

    float timeInState = stateTimer.getElapsedTime().asMilliseconds();
    float alpha_float_cos = 0.3f * cos(timeInState / 300.0f) + .7f;
    auto alpha_accept = uint8_t(alpha * alpha_float_cos * 255.0f);


    agreementText.setFillColor(sf::Color(255, 255, 255, alpha_accept));

    agreementText.setCharacterSize(30);
    agreementText.setString(button);

    float width = agreementText.getLocalBounds().width;

    agreementText.setPosition((window.getSize().x - width) / 2.0f, window.getSize().y - margin - 30);
    window.draw(agreementText);

    agreementText.setFillColor(sf::Color(255, 255, 255, 255 * alpha));

    agreementText.setCharacterSize(50);
    agreementText.setString(title);

    width = agreementText.getLocalBounds().width;

    agreementText.setPosition((window.getSize().x - width) / 2.0f, margin);
    window.draw(agreementText);

    agreementText.setCharacterSize(30);

    boost::split(blocks,
                 agreement,
                 boost::is_any_of("\n"));

    for (size_t b = 0; b < blocks.size(); b++) {
        u_int8_t blocklines = 0;
        std::wstring line;
        std::vector<std::wstring> words;

        boost::split(words,
                     blocks[b],
                     boost::is_any_of(" "));

        for (size_t i = 0; i-blocklines < words.size(); i++) {
            auto oldLine = line;
            line = line + words[i-blocklines] + L" ";
            agreementText.setString(line);
            float lineWidth = agreementText.getLocalBounds().width;
            if (lineWidth > window.getSize().x - (margin * 2)) {
                agreementText.setString(oldLine);
                agreementText.setPosition(margin, (lineSpacing * (lines + blocklines)) + marginTop);
                window.draw(agreementText);
                line.clear();
                blocklines++;
            }
        }

        agreementText.setString(line);
        agreementText.setPosition(margin, (lineSpacing * (lines + blocklines)) + marginTop);
        window.draw(agreementText);

        lines += blocklines;
        lines++;
    }
}

void BoothGui::drawCountdown() {
    if (currentState == STATE_AGREEMENT) {
      return;
    }

    countdownMutex.lock();
    int localCountdown = countdown;
    countdownMutex.unlock();

    if (localCountdown > -1) {
        countdownText.setString(std::to_string(localCountdown));
        countdownText.setPosition((window.getSize().x - countdownText.getLocalBounds().width) / 2,
                                  countdownText.getLocalBounds().height / 2.0f);
        window.draw(countdownText);
    }
}

void BoothGui::drawAlerts() {

    if (currentState == STATE_AGREEMENT) {
        alertTimer.restart();
        return;
    }

    boost::unique_lock<boost::mutex> lk(alertMutex);

    const auto  count = alerts.size();
    auto        now = alertTimer.getElapsedTime().asMilliseconds();
    int         row = 0;
    const int   offset_x = 35,
                offset_y = 20,
                spacing_x = 90,
                spacing_y = 10;
    const int   row_height = iconText.getCharacterSize() + spacing_y;

    float       alpha = min(1.0f, now / 300.0f);
    sf::Int32   endTime = 0;

    if (count < 1)
        return;

    for (auto &alert : alerts) {
        if (alert.second.endTime == 0) {
            endTime = 0;
            break;
        }

        if (alert.second.endTime > endTime)
            endTime = alert.second.endTime;
    }

    if (endTime > 0) {
        alpha = min(alpha, max(0, endTime - now) / 300.0f);
    }

    // Darken the screen
    sf::RectangleShape blackout(sf::Vector2f(window.getSize().x, window.getSize().y));
    blackout.setFillColor(sf::Color(0, 0, 0, (uint8_t) (75 * alpha)));
    window.draw(blackout);

    // Draw a frame for the alerts
    sf::RectangleShape background(sf::Vector2f(window.getSize().x, (row_height * count) + (offset_y * 2)));
    background.setFillColor(sf::Color(255, 255, 255, (uint8_t) (255 * alpha)));
    window.draw(background);

    vector <ALERT_TYPE> alertsToRemove;

    // Draw the alerts
    for (auto &alert : alerts) {
        int y = row_height * row;
        sf::Color color;

        if (alert.second.hint) {
            color = sf::Color(75, 75, 75);
        } else {
            color = COLOR_ALERT;
        }

        color.a = (uint8_t) (255 * alpha);

        iconText.setFillColor(color);
        alertText.setFillColor(color);

        iconText.setPosition(offset_x, y + offset_y + 8);
        alertText.setPosition(offset_x + spacing_x, y + offset_y);

        iconText.setString(alertTypeToString.at(alert.first));
        alertText.setString(alert.second.text);

        window.draw(iconText);
        window.draw(alertText);

        // Should we remove this alert?
        if (alert.second.endTime != 0 && now >= alert.second.endTime) {
            alertsToRemove.push_back(alert.first);
        }

        row++;
    }

    // Remove old alerts
    for (auto &alert : alertsToRemove) {
        removeAlert(alert, true);
    }
}

void BoothGui::drawDebug() {
    if (!debug)
        return;

    sf::String debugStr = "";
    debugStr += "Drawing Fps:";
    debugStr += to_string(renderFrameCounter.fps);
    debugStr += "\n";
    debugStr += "Camera Fps:";
    debugStr += to_string(cameraFrameCounter.fps);
    debugStr += "\n";

    debugLogQueueMutex.lock();
    for (const auto &string : debugLogQueue)
        debugStr += string + "\n";
    debugLogQueueMutex.unlock();

    debugText.setString(debugStr);

    window.draw(debugText);
}


float BoothGui::easeOutSin(float t, float b, float c, float d) {
    return static_cast<float>(c * sin(t / d * (M_PI / 2)) + b);
}

void BoothGui::showAgreement() {
    if (currentState != STATE_INIT) {
        setState(STATE_AGREEMENT);
    } else {
        shouldShowAgreement = true;
    }
}


void BoothGui::hideAgreement()  {
    if (currentState != STATE_AGREEMENT)
        return;

    setState(STATE_TRANS_AGREEMENT);
}


void BoothGui::updateCountdown(int new_countdown) {
    countdownMutex.lock();
    countdown = new_countdown;
    countdownMutex.unlock();
}

bool BoothGui::hasAlert(ALERT_TYPE type) {
    boost::unique_lock<boost::mutex> lk(alertMutex);

    if (alerts.empty())
        return false;

    auto alert = alerts.find(type);

    return alert != alerts.end();
}

void BoothGui::addAlert(ALERT_TYPE type, std::wstring text, bool autoRemove, bool isHint) {
    boost::unique_lock<boost::mutex> lk(alertMutex);

    if (alerts.empty())
        alertTimer.restart();

    removeAlert(type, true);

    sf::Int32 startTime = alertTimer.getElapsedTime().asMilliseconds();
    sf::Int32 endTime = 0;

    if (autoRemove) {
        if (isHint) {
            endTime = startTime + 3000;
        } else  {
            endTime = startTime + 10000;
        }
    }

    alerts.insert(std::make_pair(type, (Alert){startTime, endTime, std::move(text), isHint}));
}

void BoothGui::removeAlert(ALERT_TYPE type, bool forced) {
    auto alert = alerts.find(type);

    if (alert == alerts.end())
        return;

    if (!forced && alert->second.endTime == 0) {
        alert->second.endTime = alertTimer.getElapsedTime().asMilliseconds() + 300;
    } else {
        alerts.erase(type);
    }

}

void BoothGui::removeAlert(ALERT_TYPE type) {
    boost::unique_lock<boost::mutex> lk(alertMutex);

    removeAlert(type, false);
}

void BoothGui::setPrinterEnabled(bool printerEnabled) {
    this->printerEnabled = printerEnabled;
}

void BoothGui::setTemplateEnabled(bool templateEnabled) {
    this->templateEnabled = templateEnabled;
}

void BoothGui::cancelPrint() {
    if (getCurrentGuiState() == STATE_FINAL_IMAGE_PRINT) {
        setState(STATE_FINAL_IMAGE_PRINT_CANCELED);
    }
}

void BoothGui::confirmPrint() {
    if (getCurrentGuiState() == STATE_FINAL_IMAGE_PRINT) {
        setState(STATE_FINAL_IMAGE_PRINT_CONFIRMED);
    }
}
void BoothGui::log(std::string s) {
    debugLogQueueMutex.lock();
    debugLogQueue.push_front(s);
    if (debugLogQueue.size() > DEBUG_QUEUE_SIZE)
        debugLogQueue.pop_back();
    debugLogQueueMutex.unlock();
}

void BoothGui::setDebugOutput(bool debug) {
    this->debug = debug;
}
