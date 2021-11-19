/*
 * Needed Libraries:
 * Adafruit NeoPixel by Adafruit
 * PinChangeInterrupt by NicoHood
 */


#include <avr/wdt.h>
#include "globals.h"
#include "FastCRC.h"
#include "State.h"
#include "OffState.h"
#include "IdleState.h"
#include "BusyState.h"


/*
 * State Machine
 */
BaseState *currentState = 0;
unsigned long lastAnimationStep = 0;

void goToState(BaseState *nextState) {
  if(nextState == currentState)
    return;
  if(currentState) {
    currentState->exit();
  }
  currentState=nextState;
  if(nextState) {
    nextState->enter();
  }
}


void onPacketReceived(const uint8_t* buffer, size_t size) {
  // Check, if we need to bootload. If so, do it. No state needed
  if(size == 1) {
    switch(buffer[0]) {
      case '0': {
        typedef void (*do_reboot_t)(void);
        const do_reboot_t do_reboot = (do_reboot_t)((FLASHEND - 511) >> 1);
        cli(); MCUSR = TCCR0A = TCCR1A = TCCR2A = 0; // make sure interrupts are off and timers are reset.
        do_reboot();    
      }
      return;
      case 'i': {
        // Identify yourself
        Serial.write('b');
        Serial.flush();    
      }
      return;
      case '.': {
        // Reset our software watchdog
        lastHeartbeat = millis();
        // no return here, we want it to propagate to the other states
        break;
      }
      break;
      default:
        break;
    }
  }
  if(currentState) {
    currentState->processCommand(buffer, size);
  }
}

void setup() {
  // Reset and disable Watchdog
  // NEVER EVER REMOVE THE FOLLOWING 2 LINES!!!!
  MCUSR = 0;
  wdt_disable();

  // Setup Pins
  pinMode(PIN_STATUS, OUTPUT);
  digitalWrite(PIN_STATUS, LOW);

 
  
  pinMode(PIN_BUTTON, INPUT); 
  digitalWrite(PIN_BUTTON, HIGH);
  pinMode(PIN_SWITCH, INPUT);
  digitalWrite(PIN_SWITCH, HIGH);
  pinMode(PIN_FLASH_CAM_TRIGGER, INPUT);
  // pullups
  digitalWrite(PIN_FLASH_CAM_TRIGGER, HIGH);

  pinMode(PIN_FLASH_ON, OUTPUT);
  // flash off by default
  digitalWrite(PIN_FLASH_ON, LOW);

  pinMode(PIN_ON, OUTPUT); 
  pinMode(PIN_LEVEL_SHIFTER_OE, OUTPUT);


  packetSerial.begin(38400);
  packetSerial.setPacketHandler(&onPacketReceived);

  // If the switch is on, turn on instantly BUT ONLY THIS FIRST TIME
  if(digitalRead(PIN_SWITCH) == LOW) {
    // We want to call exit function but not enter function
    currentState = &OffState::INSTANCE;
    goToState(&BootingState::INSTANCE);
  } else {
    goToState(&OffState::INSTANCE);
  }
}

void loop() { 
  bool checkHeartbeat = currentState == 0 || currentState->needsHeartbeat();
  
  if(checkHeartbeat && millis() - lastHeartbeat > 15000) {
    // Turn off if heartbeat was not sent
    goToState(&OffState::INSTANCE);
    return;
  }
  
 if(currentState) {
  BaseState *nextState = currentState->logicStep();
  unsigned long dt = millis() - lastAnimationStep;
  if(dt > 33) {
    packetSerial.update();
    lastAnimationStep = millis();
    // approx 30fps
    currentState->animationStep(dt);
  }
  goToState(nextState);
 }
 packetSerial.update();
}
