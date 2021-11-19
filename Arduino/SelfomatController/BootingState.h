#ifndef BOOTING_STATE_H
#define BOOTING_STATE_H

#include "State.h"
#include "globals.h"
#include "Arduino.h"

class BootingState: public BaseState {
private:
  uint32_t frame = 0;  
  int b = 0;
  bool shouldExit;
  bool exitAnimationDone;
public:  
  static BootingState INSTANCE;
  /**
   * Handle a serial command.
   * @param buffer: the command buffer
   * @param size: buffer length
   * 
   * @returns: true, if the command was handled
   */
  virtual bool processCommand(const uint8_t* buffer, size_t size);
  virtual void animationStep(unsigned long dt);
  
  /**
   * Do some polling logic
   * @returns the pointer to the next state (can return itself)
   */
  virtual BaseState* logicStep();
  /**
   * Called ONCE on state enter
   */
  virtual void enter();
  
  /**
   * Called ONCE before state exits
   */
  virtual void exit();

  virtual bool needsHeartbeat();
};

#endif
