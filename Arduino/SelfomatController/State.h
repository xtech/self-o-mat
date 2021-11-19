#ifndef STATE_H
#define STATE_H
#include "globals.h"
#include "stdint.h"
#include "Arduino.h"
#include "EEPROM.h"

/**
 * BaseState definition
 */
class BaseState {

private:
  // Set this variable in order to force a state.
  // Good for states which are reachable from every other state.
  // E.g. Agreement
  static BaseState *forcedNextState;;
  unsigned long millisEnteredState;
protected:
  unsigned long timeInState();

  void blink(uint8_t count);

  void readSettings();
  void writeSettings();

  // Updates the dependencies (e.g. ring) using the new settings
  void updateSettingDependencies();

  void sendCurrentSettings();

  bool checkSettings(struct settings* settings_ptr);

  void sendCommand(const char command);

  void sendCommand(const char *command, uint8_t length);

  void showCenterLed(uint16_t duration);
  
public:
  /**
   * Handle a serial command.
   * @param buffer: the command buffer
   * @param size: buffer length
   * 
   * @returns: true, if the command was handled
   */
  virtual bool processCommand(const uint8_t* buffer, size_t size);
  
  virtual void animationStep(unsigned long dt) = 0;
  /**
   * Do some polling logic
   * @returns the pointer to the next state (can return itself).
   */
  virtual BaseState* logicStep();
  /**
   * Called ONCE on state enter
   */
  virtual void enter();
  /**
   * Called ONCE before state exits
   */
  virtual void exit() = 0;

  // Return true, if this state needs a heartbeat in order to work.
  virtual bool needsHeartbeat() = 0;
};

#endif
