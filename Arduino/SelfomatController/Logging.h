#ifndef LOGGING_H
#define LOGGING_H

#include "Print.h"

//#define LOGGING_ACTIVE /* Comment out when logging is unwanted; could also overwrite class! */

class Logging : public Print {
public:
  Logging(uint8_t transmitPin);

  void begin(uint16_t baudRate=38400);

  size_t write(uint8_t txChar);

private:
  void* pSwSerial;
};

#endif
