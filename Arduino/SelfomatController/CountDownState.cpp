#include "CountDownState.h"
#include "BusyState.h"

CountDownState CountDownState::INSTANCE;

bool CountDownState::processCommand(const uint8_t* buffer, size_t size) {
  // Process base commands. No fancy extras needed
  return BaseState::processCommand(buffer, size);
}

void CountDownState::animationStep(unsigned long dt) {
  uint32_t color = colors[animationCycle];
  float percentage = ((float)timeInState() - animationCycle * oneCycleMillis) / oneCycleMillis;
  
  float pixelBorder = ring.numPixels() - ring.numPixels() * percentage;
  
  for(int8_t i=0; i<ring.numPixels(); i++) {

    if(i > pixelBorder) {
      ring.setPixelColor((i+settings.ledOffset)%ring.numPixels(), color);
    } else {
      ring.setPixelColor((i+settings.ledOffset)%ring.numPixels(), lastColor);
    }
  }
  ring.show();

  if(percentage >= 1.0f) {
    lastColor = color;
    animationCycle++;
    if(animationCycle > 2) {
      animationCycle = 2;
    }
  }
}

BaseState* CountDownState::logicStep() {
  if(timeInState() > settings.countDownMillis) {
    // Trigger the capture and go to idle
    sendCommand('t');
    return &BusyState::INSTANCE;
  }
  return this;
}

void CountDownState::enter() {
  BaseState::enter();
  oneCycleMillis = (settings.countDownMillis)/3.0f;
  animationCycle = 0;
  lastColor = 0;
}

void CountDownState::exit() {
    for(int8_t i=0; i<ring.numPixels(); i++) {
      ring.setPixelColor(i, 0);
    }
    ring.show();
}

bool CountDownState::needsHeartbeat() {
  return !heartbeatDeactivated;
}
