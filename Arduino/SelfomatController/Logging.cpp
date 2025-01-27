#include "Logging.h"

#ifdef LOGGING_ACTIVE
  #include <NeoSWSerial.h>
  #ifndef NEOSWSERIAL_EXTERNAL_PCINT
  #error NEOSWSERIAL_EXTERNAL_PCINT needs to be defined inside of NeoSWSerial.h or as compile flag.
  #endif
#endif

Logging::Logging(uint8_t transmitPin) {
#ifdef LOGGING_ACTIVE
  /* using the same pin for RX and TX seems to work; we won't RX anyways */
  pSwSerial = new NeoSWSerial(transmitPin, transmitPin);
#endif
}

void Logging::begin(uint16_t baudRate) {
#ifdef LOGGING_ACTIVE
  ((NeoSWSerial*)pSwSerial)->begin(baudRate);
  ((NeoSWSerial*)pSwSerial)->ignore(); // do not use RX
#endif
}

size_t Logging::write(uint8_t txChar) {
#ifdef LOGGING_ACTIVE
  return ((NeoSWSerial*)pSwSerial)->write(txChar);
#else
  return 0;
#endif
}
