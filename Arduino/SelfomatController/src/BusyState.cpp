#include "BusyState.h"
#include "IdleState.h"
#include "PrintingState.h"

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

  
  
  // timeout in busy state
  if(timeInState() > 15000 || exitIdle) {
    return &IdleState::INSTANCE;
  }
  if(exitPrint) {
    return &PrintingState::INSTANCE;
  }
  return this;
}

void BusyState::enter() {
  BaseState::enter();
  flashTriggered = exitPrint = exitIdle = false;
}

void BusyState::exit() {
}

bool BusyState::needsHeartbeat() {
  return !heartbeatDeactivated;
}

