#include "BootingState.h"
#include "IdleState.h"

BootingState BootingState::INSTANCE;

bool BootingState::processCommand(const uint8_t* buffer, size_t size) {
  bool parentHandled = BaseState::processCommand(buffer, size);
  // If it was a heartbeat, we want to exit
  if (size == 1 && buffer[0] == '.') {
    shouldExit = true;
    return true;
  }
  return parentHandled;
}

void BootingState::animationStep(unsigned long dt) {
  // Fade
  if (shouldExit) {
    if (b > 0) {
      b-=min(b,4);
    } else {
      exitAnimationDone = true;
    }
  } else {
    if (b < 255) {
      b+=min(4, 255-b);
    }
  }

  // Animate
  for (int i = 0; i < ring.numPixels(); i++) {
    byte diff = (i - frame) % ring.numPixels();
    byte color = (b / ring.numPixels() * diff);
    ring.setPixelColor(i, 0, color, color >> 2);
  }
  ring.show();

  // Skip to next frame
  frame++;
}

BaseState* BootingState::logicStep() {
  BaseState* parentStep = BaseState::logicStep();
  if(parentStep)
    return parentStep;

  if(shouldExit && exitAnimationDone) {
    logger.println( F("exit to -> IdleState") );
    return &IdleState::INSTANCE;
  }

  return this;
}

void BootingState::enter() {
  frame = 0;
  exitAnimationDone = shouldExit = false;
}

void BootingState::exit() {
  // Booting done, we need a heartbeat. Assume we got it now
  heartbeatDeactivated = false;
  lastHeartbeat = millis();
}

bool BootingState::needsHeartbeat() {
  // Booting so no heartbeat required
  return false;
}
