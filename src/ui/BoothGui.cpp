//
// Created by clemens on 21.01.19.
//

#include "BoothGui.h"

using namespace std;


BoothGui::BoothGui() : debugLogQueue(), stateTimer() {
    videoMode = sf::VideoMode(1280, 800);
    currentState = STATE_INIT;
}

BoothGui::~BoothGui() {
}

bool BoothGui::start() {
    // Load assets
    if (!hackFont.loadFromFile("./assets/Hack-Regular.ttf")) {
        cerr << "Could not load font." << endl;
        return false;
    }

    if (!textureLiveOverlay.loadFromFile("./assets/live.png")) {
        cerr << "Could not load live asset." << endl;
        return false;
    }
    imageSpriteLiveOverlay.setTexture(textureLiveOverlay);

    if (!textureFinalImageOverlay.loadFromFile("./assets/template_screen.png")) {
        cerr << "Could not load screen template asset." << endl;
        return false;
    }
    imageSpriteFinalOverlay.setTexture(textureFinalImageOverlay);

    if (!texturePrintOverlay.loadFromFile("./assets/print_overlay.png")) {
        cerr << "Could not load print template asset." << endl;
        return false;
    }
    imageSpritePrintOverlay.setTexture(texturePrintOverlay);


    // Read properties for the template
    boost::property_tree::ptree ptree;
    try {
        boost::property_tree::read_json("./assets/template_screen_props.json", ptree);
        finalOverlayOffsetTop = ptree.get<int>("offset_top");
        finalOverlayOffsetLeft = ptree.get<int>("offset_left");
        finalOverlayOffsetRight = ptree.get<int>("offset_right");
        finalOverlayOffsetBottom = ptree.get<int>("offset_bottom");
    } catch (boost::exception &e) {
        logError(std::string("Error loading template properties: ") + boost::diagnostic_information(e));
        return false;
    }


    rect_overlay = sf::RectangleShape(sf::Vector2f(videoMode.width, videoMode.height));
    count_down_circle = sf::CircleShape(19.0f);

    isRunning = true;
    renderThreadHandle = boost::thread(boost::bind(&BoothGui::renderThread, this));
    return true;
}

void BoothGui::stop() {
    isRunning = false;
    std::cout << "Waiting for gui to stop" << std::endl;
    renderThreadHandle.join();
}

void BoothGui::updatePreviewImage(void *data, uint32_t width, uint32_t height) {
    if (currentState == STATE_TRANS_PRINT_PREV1)
        return;
    imageMutex.lock();
    imageBuffer = data;
    imageWidth = width;
    imageHeight = height;
    imageDirty = true;
    imageShown = true;
    imageMutex.unlock();
}


void BoothGui::renderThread() {
    cout << "Render thread started!" << endl;
    sf::ContextSettings settings;
    settings.antialiasingLevel = 8;
    window.create(videoMode, "self-o-mat", sf::Style::Default, settings);

    window.setVerticalSyncEnabled(true);



    // Init variables
    clearColor = sf::Color(0, 0, 0);
    debugText.setFont(hackFont);
    debugText.setFillColor(sf::Color::Black);
    debugText.setOutlineColor(sf::Color::White);
    debugText.setOutlineThickness(1.5);
    debugText.setCharacterSize(15);

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
                        cout << "Recreating texture to fit the new image" << endl;
                        imageTexture.create(imageWidth, imageHeight);
                    }
                }


                // TODO: needed?
                //imageSprite.setTexture(imageTexture, true);
                //imageSprite.setTextureRect(sf::IntRect(0, 0, imageHeight, imageWidth));

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


                float finalOverlayCenterX =
                        ((float) finalOverlayOffsetLeft + (float) finalOverlayOffsetRight) / 2.0f;
                float finalOverlayCenterY =
                        ((float) finalOverlayOffsetTop + (float) finalOverlayOffsetBottom) / 2.0f;


                finalImageSprite.setTexture(imageTexture, true);
                finalImageSprite.setTextureRect(sf::IntRect(0, 0, imageWidth, imageHeight));

                // Same for the final sprite
                float finalScaleX = (float) (finalOverlayOffsetRight - finalOverlayOffsetLeft) / (float) imageWidth;
                float finalScaleY =
                        (float) (finalOverlayOffsetBottom - finalOverlayOffsetTop) / (float) imageHeight;
                float finalScale = max(finalScaleX, finalScaleY);
                finalImageSprite.setScale(finalScale, finalScale);

                float finalImageCenterX = (float) imageWidth * finalScale * 0.5f;
                float finalImageCenterY = (float) imageHeight * finalScale * 0.5f;

                finalImageSprite.setPosition(finalOverlayCenterX - finalImageCenterX,
                                             finalOverlayCenterY - finalImageCenterY);





                imageTexture.update((sf::Uint8 *) imageBuffer, imageWidth, imageHeight, 0, 0);
                imageDirty = false;
            }
            imageMutex.unlock();
        }


        switch (currentState) {
            case STATE_LIVE_PREVIEW:
                if (imageShown) {
                    // We can show a preview image
                    window.draw(imageSprite);


                    float alpha_float = 0.4f * cos(float(stateTimer.getElapsedTime().asMilliseconds()) / 800.0f) + .6f;
                    auto alpha = uint8_t(alpha_float * 255.0f);

                    imageSpriteLiveOverlay.setColor(sf::Color(255, 255, 255, alpha));
                    window.draw(imageSpriteLiveOverlay);
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
                window.draw(imageSpriteFinalOverlay);

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
                window.draw(imageSpriteFinalOverlay);
                float duration = 1000.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                if (timeInState >= duration) {
                    setState(STATE_TRANS_FINAL_IMAGE_PRINT);
                }
            }
                break;
            case STATE_TRANS_FINAL_IMAGE_PRINT: {
                window.draw(finalImageSprite);
                window.draw(imageSpriteFinalOverlay);

                float duration = 350.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float linearPercentage = min(1.0f, timeInState / duration);


                // Add easing to the percentage
                float percentage = easeOutSin(linearPercentage, 0.0f, 1.0f, 1.0f);


                float templateY = window.getSize().y - percentage * texturePrintOverlay.getSize().y;

                imageSpritePrintOverlay.setPosition(0, templateY);

                window.draw(imageSpritePrintOverlay);


                for(int i = 0; i < 6; i++) {
                    count_down_circle.setPosition(339.0f + i*(113.0f),templateY + 149.0f- 19.0f);
                    count_down_circle.setFillColor(sf::Color(155, 194, 189));

                    window.draw(count_down_circle);
                }

                if (timeInState >= duration) {
                    setState(STATE_FINAL_IMAGE_PRINT);
                }
            }
                break;
            case STATE_FINAL_IMAGE_PRINT: {
                window.draw(finalImageSprite);
                window.draw(imageSpriteFinalOverlay);
                window.draw(imageSpritePrintOverlay);

                float duration = 3500.0f;
                float timeInState = stateTimer.getElapsedTime().asMilliseconds();

                float templateY = window.getSize().y - texturePrintOverlay.getSize().y;

                for(int i = 0; i < 6; i++) {
                    count_down_circle.setPosition(340.0f + i*(113.0f),templateY + 149.0f - 19.0f);
                    if(timeInState >= 500.0 * (i+1)) {
                        count_down_circle.setFillColor(sf::Color(20, 64, 66, 255));
                    } else {
                        count_down_circle.setFillColor(sf::Color(155, 194, 189));
                    }

                    window.draw(count_down_circle);
                }
            }
                break;
            case STATE_TRANS_PRINT_PREV1: {
                float duration = 250.0f;

                float timeInState = stateTimer.getElapsedTime().asMilliseconds();
                float percentage = timeInState / duration;


                float alpha = max(0.0f, min(255.0f, percentage * 255.0f));
                window.draw(finalImageSprite);
                window.draw(imageSpriteFinalOverlay);
                window.draw(imageSpritePrintOverlay);

                float templateY = window.getSize().y - texturePrintOverlay.getSize().y;

                for(int i = 0; i < 6; i++) {
                    count_down_circle.setPosition(340.0f + i*(113.0f),templateY + 149.0f- 19.0f);
                    count_down_circle.setFillColor(sf::Color(20, 64, 66, 255));

                    window.draw(count_down_circle);
                }

                rect_overlay.setFillColor(sf::Color(0, 0, 0, alpha));
                window.draw(rect_overlay);

                if (timeInState >= duration) {
                    imageShown = false;
                    setState(STATE_TRANS_FINAL_PREV2);
                }
            }
                break;
            case STATE_TRANS_FINAL_PREV2: {
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
            case STATE_INIT:
                window.clear(sf::Color(0, 255, 0));
                break;
            default:
                window.clear(sf::Color(255, 0, 0));
                break;
        }

        drawDebug();


        // Draw the window
        window.display();
    }
    window.close();
}

void BoothGui::setState(BoothGui::GUI_STATE newState) {
    currentState = newState;
    stateTimer.restart();
}

void BoothGui::initialized() {
    // Move the state from initialized to preview live
    setState(STATE_LIVE_PREVIEW);
}

void BoothGui::log(int level, std::string s) {
    debugLogQueueMutex.lock();
    std::stringstream stream;
    stream << "[" << level << "]" << ": " << s;
    cout << stream.str() << endl;
    debugLogQueue.push_front(stream.str());
    if (debugLogQueue.size() > DEBUG_QUEUE_SIZE)
        debugLogQueue.pop_back();
    debugLogQueueMutex.unlock();
}


void BoothGui::drawDebug() {
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
