#include "globals.h"

struct settings settings = {0};

struct settings default_settings = {
  .ledTypeIndex = 0, // RGB
  .ledCount = 16, // Default for our box
  .ledOffset = 0, // We can't know this
  .countDownMillis = 3000, // Seems reasonable
  .flashDurationMicros = 10000,
  .flashMode = 0,
  .maxLedBrightness = 255,
  .crcChecksum = 0
};

bool settingsDirty = true;

neoPixelType supportedPixels[PIXEL_TYPES] = {
  (NEO_GRB | NEO_KHZ800),
  (NEO_GRBW | NEO_KHZ800)
};

Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, PIN_LED, supportedPixels[0]);

FastCRC16 CRC16;

COBSSpacePacketSerial packetSerial;

Logging logger(PIN_SWSERIAL_LOGGING_TX);

unsigned long lastHeartbeat = 0;
bool heartbeatDeactivated = false;
