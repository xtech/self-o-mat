#ifndef GLOBALS_H
#define GLOBALS_H
#include "stdint.h"
#include <Adafruit_NeoPixel.h>
#include "FastCRC.h"
#include "PacketSerial.h"

typedef PacketSerial_<COBS, ' '> COBSSpacePacketSerial;

/*
 * PIN Defines
 */
#define PIN_NEOPIXELS 4

// Status LED for blinks
#define PIN_STATUS LED_BUILTIN

// DO NOT CHANGE!!! WE HAVE DIRECT HARDWARE ACCESS ON THIS PIN!!!!
// #define PIN_FLASH_ON 11
// #define PIN_FLASH_CAM_TRIGGER 7

#define PIN_BUTTON 2
#define PIN_SWITCH 3
#define PIN_ON 8
#define PIN_LEVEL_SHIFTER_OE 19

#define PIXEL_TYPES 2
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

extern unsigned long lastHeartbeat;
extern bool heartbeatDeactivated;

#endif
