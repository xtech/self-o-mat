#ifndef GLOBALS_H
#define GLOBALS_H
#include "stdint.h"
#include "Logging.h"
#include <Adafruit_NeoPixel.h>
#include "FastCRC.h"
#include "PacketSerial.h"

/*
 * PIN Defines
 */
#define PIN_LED                 (6) //(4) TODO/FIXME: change back to 4 at least during review as this has been adapter for some special hardware
#define PIN_LED_OFF             (9)
#define PIN_STATUS              (20)

// DO NOT CHANGE!!! WE HAVE DIRECT HARDWARE ACCESS ON THIS PIN!!!!
#define PIN_FLASH_ON            (11)
#define PIN_FLASH_CAM_TRIGGER   (7)

#define PIN_BUTTON              (2)
#define PIN_SWITCH              (3)
#define PIN_ON                  (8)
#define PIN_LEVEL_SHIFTER_OE    (19)

#define PIN_SWSERIAL_LOGGING_TX (13)

#define PIXEL_TYPES             (2)
extern neoPixelType supportedPixels[PIXEL_TYPES];




/**
 * Setting Defines
 */

#pragma pack(push, 1)

struct settings {
    uint8_t ledTypeIndex;
    uint8_t ledCount;
    uint8_t ledOffset;
    uint16_t countDownMillis;
    uint32_t flashDurationMicros;
    uint8_t flashMode;
    uint8_t maxLedBrightness;
    uint16_t crcChecksum;
} __attribute__((packed));

extern struct settings settings;
extern struct settings default_settings;
extern bool settingsDirty;

#pragma pack(pop)

extern Adafruit_NeoPixel ring;
extern FastCRC16 CRC16;
extern COBSSpacePacketSerial packetSerial;
extern Logging logger;

extern unsigned long lastHeartbeat;
extern bool heartbeatDeactivated;

#endif
