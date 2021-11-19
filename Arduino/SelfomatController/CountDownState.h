#ifndef COUNT_DOWN_STATE_H
#define COUNT_DOWN_STATE_H

#include "State.h"
#include "globals.h"
#include "Arduino.h"

class CountDownState : public BaseState {
private:
  uint8_t animationCycle;
  float oneCycleMillis;
  uint32_t lastColor;

  uint32_t colors[3] = {0xFF0000, 0xFF8800, 0x00FF00};
public:  
  static CountDownState INSTANCE;
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
