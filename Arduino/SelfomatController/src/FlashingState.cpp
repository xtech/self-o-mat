#include "FlashingState.h"
#include "IdleState.h"

FlashingState FlashingState::INSTANCE;

bool FlashingState::processCommand(const uint8_t* buffer, size_t size) {
  // Handle '.' since it's the only way to exit
  if(size == 1 && buffer[0] == '.') {
    shouldExit = true;
    return true;
  }
  // Handle 'f' since our gui will spam until it hears from us
  if(size == 1 && buffer[0] == 'f') {
    sendCommand('F');
  }
  // Nothing else
  return false;
}

void FlashingState::animationStep(unsigned long dt) {

    // Flash 10x per second
    int on = (timeInState() / 150) & 0x01;
  
    for(int j = 0; j < ring.numPixels(); j++) {
      if(on) {
        ring.setPixelColor(j, 255, 0, 0);
      } else {
        ring.setPixelColor(j, 0, 0, 0);
      }
    }
    ring.show();
}

BaseState* FlashingState::logicStep() {
  if(shouldExit) {
    sendCommand('a');
    return &IdleState::INSTANCE;
  }
  return this;
}

void FlashingState::enter() {
  BaseState::enter();
  shouldExit = false;
}

void FlashingState::exit() {

}

bool FlashingState::needsHeartbeat() {
  // Plz no
  return false;
}
