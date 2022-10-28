#include "AgreementState.h"
#include "IdleState.h"

AgreementState AgreementState::INSTANCE;

bool AgreementState::processCommand(const uint8_t* buffer, size_t size) {
  // Handle 'a' since we're already in agreement
  if(size == 1 && buffer[0] == 'a') {
    sendCommand('A');
    return true;
  }

  // Process base commands.
  if(BaseState::processCommand(buffer, size))
    return true;

  return false;
}

void AgreementState::animationStep(unsigned long dt) {
    // Fade
    if (shouldExit) {
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
      ring.setPixelColor(j, 0, brightness, 0);
    }
    ring.show();
}

BaseState* AgreementState::logicStep() {
  if(!shouldExit && digitalRead(PIN_BUTTON) == LOW) {
    shouldExit = true;
    sendCommand('c');
  }

  if(shouldExit && exitAnimationDone) {
    sendCommand('a');
    logger.println( F("exit to -> IdleState") );
    return &IdleState::INSTANCE;
  }
  return this;
}

void AgreementState::enter() {
  BaseState::enter();
  b = 0;
  frame = 0;
  exitAnimationDone = shouldExit = false;
  sendCommand('A');
}

void AgreementState::exit() {

}

bool AgreementState::needsHeartbeat() {
  return !heartbeatDeactivated;
}
