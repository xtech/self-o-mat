#include "OffState.h"

OffState OffState::INSTANCE;

bool OffState::processCommand(const uint8_t* buffer, size_t size) {
  // We drop serial commands in this state
  return false;
}
void OffState::animationStep(unsigned long dt) {
  // We have no animation in this state
}

BaseState* OffState::logicStep() {
  allowExitState |= timeInState() > 3000;

  if (!allowExitState)
    return this;

  if (digitalRead(PIN_SWITCH) == LOW) {
    // The switch is on and we're allowed to exit the off state->do it
    return &BootingState::INSTANCE;
  }
  return this;
}

void OffState::enter() {
  BaseState::enter();
  
  // read settings
  readSettings();
  
  allowExitState = false;


  blink(3);
  // Disable the ring
  ring.setPin(PIN_LED_OFF);
  digitalWrite(PIN_LED, LOW);

  // power off the system and level shifter
  digitalWrite(PIN_LEVEL_SHIFTER_OE, LOW);
  digitalWrite(PIN_ON, LOW);
}

void OffState::exit() {
  // read settings
  readSettings();
  heartbeatDeactivated = false;
  // If we exit the off state, we need to power the system and level shifter
  digitalWrite(PIN_ON, HIGH);
  digitalWrite(PIN_LEVEL_SHIFTER_OE, HIGH);


  // Setup the ring
  ring.setPin(PIN_LED);
  ring.begin();
  ring.clear();
  ring.show();

  blink(2);
}

bool OffState::needsHeartbeat() {
  // Obviously
  return false;
}
