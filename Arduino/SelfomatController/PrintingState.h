#ifndef PRINTING_STATE_H
#define PRINTING_STATE_H

#include "State.h"
#include "globals.h"
#include "Arduino.h"

class PrintingState : public BaseState {
private:
  uint8_t frame;  
  float b;
  bool shouldExit;
  bool exitAnimationDone;
  bool cancelSent;
public:  
  static PrintingState INSTANCE;
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
