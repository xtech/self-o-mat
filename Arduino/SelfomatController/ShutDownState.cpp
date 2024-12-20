#include "ShutDownState.h"
#include "OffState.h"

ShutDownState ShutDownState::INSTANCE;

bool ShutDownState::processCommand(const uint8_t* buffer, size_t size) {
  return false;
}

void ShutDownState::animationStep(unsigned long dt) {

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
    ring.setPixelColor(i, color, 0, 0);
  }
  ring.show();

  // Skip to next frame
  frame++;
}

BaseState* ShutDownState::logicStep() {
  // We never exit. This state will be exited using the heartbeat timeout
  return this;
}

void ShutDownState::enter() {
  BaseState::enter();
  logger.println( F("Entering ShutDownState") );
  writeSettings();
  b = 0;
  frame = 0;
  exitAnimationDone = shouldExit = false;
  sendCommand('d');

  // Shutdown state, enable heartbeat and reset. We'll shut off as soon as no heartbeats arrive + timeout
  lastHeartbeat = millis();
}

void ShutDownState::exit() {
  logger.println( F("Leaving ShutDownState") );
}

bool ShutDownState::needsHeartbeat() {
  // yes
  return true;
}
