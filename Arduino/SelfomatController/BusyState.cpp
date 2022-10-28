#include "BusyState.h"
#include "IdleState.h"
#include "PrintingState.h"
#include "PinChangeInterrupt.h"

BusyState BusyState::INSTANCE;

bool BusyState::processCommand(const uint8_t* buffer, size_t size) {
  if(BaseState::processCommand(buffer, size))
    return true;

  if(size == 1) {
    switch(buffer[0]) {
      case 'k':
        exitIdle = true;
        return true;
      case 'p':
        exitPrint = true;
        return true;
      default:
        return false;
    }
  }
  return false;
}

void BusyState::animationStep(unsigned long dt) {
  // No animation here!
}

BaseState* BusyState::logicStep() {

  if(flashTriggered) {
    if(settings.flashDurationMicros < 0) {
      delay(10);
    } else {
      while(micros() - flashStartMicros < settings.flashDurationMicros);
    }
    digitalWrite(PIN_FLASH_ON, LOW);
    flashTriggered = false;
  }

  // timeout in busy state
  if(timeInState() > 15000 || exitIdle) {
    logger.println(  F("exit to -> IdleState") );
    return &IdleState::INSTANCE;
  }
  if(exitPrint) {
    logger.println( F("exit to -> PrintingState") );
    return &PrintingState::INSTANCE;
  }
  return this;
}

void BusyState::enter() {
  BaseState::enter();
  flashTriggered = exitPrint = exitIdle = false;
  attachPinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_FLASH_CAM_TRIGGER), triggerFlash, FALLING);
}

void BusyState::exit() {
  // first, detach the interrupt!!
  detachPinChangeInterrupt(digitalPinToPinChangeInterrupt(PIN_FLASH_CAM_TRIGGER));
  // then turn off the light for safety
  digitalWrite(PIN_FLASH_ON, LOW);
}

bool BusyState::needsHeartbeat() {
  return !heartbeatDeactivated;
}


void BusyState::triggerFlash() {
  PORTB |= 0b1000;
  flashTriggered = true;
  flashStartMicros = micros();
}
