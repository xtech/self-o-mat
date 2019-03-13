//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_FPSCOUNTER_H
#define SELF_O_MAT_FPSCOUNTER_H


#include <SFML/System/Clock.hpp>
#include <cstdint>

class FPSCounter {
private:
    sf::Clock fpsClock;
    uint32_t frameCounter = 0;
public:
    float fps;
    void nextFrame() {
        // Called once per frame so we increment the frame counter here
        frameCounter++;
        sf::Int32 elapsedTime = fpsClock.getElapsedTime().asMilliseconds();
        if(elapsedTime > 1000) {
            fpsClock.restart();
            fps = 1000.0f * (float)frameCounter / (float)elapsedTime;
            frameCounter = 0;
        }
    }
};


#endif //SELF_O_MAT_FPSCOUNTER_H
