#include "PrintingState.h"
#include "IdleState.h"

PrintingState PrintingState::INSTANCE;

bool PrintingState::processCommand(const uint8_t* buffer, size_t size) {
  // Process base commands. No fancy extras needed
  if(BaseState::processCommand(buffer, size))
    return true;

  if(size == 1 && buffer[0] == 'k') {
    shouldExit = true;
  }
}

void PrintingState::animationStep(unsigned long dt) {
    // Fade
    if (shouldExit || cancelSent) {
      if (b > 0.0) {
        b -= min(b, 0.025);
      } else {
        exitAnimationDone = true;
      }
    } else {
      if (b < 1.0) {
        b += min(1-b, 0.01);
      }
    }

  
    float angleOffset = timeInState()/300.0f;
    float anglePerLed = (2.0 * PI) / ring.numPixels();
    for(int j = 0; j < ring.numPixels(); j++) {
      float angle = (float)j * anglePerLed + angleOffset;   
      float sin_x = sin(angle);
      uint8_t brightness = sin_x*sin_x * 255.0f * b;
      ring.setPixelColor(j, 0, brightness, brightness >> 2);
    }
    ring.show();
}

BaseState* PrintingState::logicStep() {
  BaseState *parentState = BaseState::logicStep();
  if(parentState)
    return parentState;

  if(!cancelSent && digitalRead(PIN_BUTTON) == LOW) {
    cancelSent = true;
    sendCommand('c');
  }
 
  if(timeInState() > 30000 || (shouldExit && exitAnimationDone)) {
    return &IdleState::INSTANCE;
  }
  return this;
}

void PrintingState::enter() {
  BaseState::enter();
  cancelSent = false;
  b = 0;
  frame = 0;
  exitAnimationDone = shouldExit = false;
}

void PrintingState::exit() {

}

bool PrintingState::needsHeartbeat() {
  return !heartbeatDeactivated;
}
