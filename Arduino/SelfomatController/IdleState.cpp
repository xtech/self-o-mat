#include "IdleState.h"
#include "CountDownState.h"
#include "FastMath.h"

IdleState IdleState::INSTANCE;

bool IdleState::processCommand(const uint8_t* buffer, size_t size) {
  if(BaseState::processCommand(buffer, size))
    return true;
  if(size == 1 && buffer[0] == 't') {
    externalTrigger = true;
  }
}

void IdleState::animationStep(unsigned long dt) {
  float sinx = isin(timeInState()>>4);
  float brightness = sinx*sinx;
  uint8_t g = brightness * 255.0f;
  for(int i = 0; i < ring.numPixels(); i++) {
    ring.setPixelColor(i, 0, g, g >> 2);
  }
  ring.show();
}

BaseState* IdleState::logicStep() {
  BaseState* parentStep = BaseState::logicStep();
  if(parentStep)
    return parentStep;

  if(digitalRead(PIN_BUTTON) == LOW || externalTrigger) {
    return &CountDownState::INSTANCE;
  }

  return this;
}

void IdleState::enter() {
  BaseState::enter();
  logger.println( F("Entering IdleState") );
  externalTrigger = false;
  green = 0;
}

void IdleState::exit() {
  logger.println( F("Leaving IdleState") );
}

bool IdleState::needsHeartbeat() {
  return !heartbeatDeactivated;
}
