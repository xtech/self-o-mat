#include "State.h"
#include "ShutDownState.h"
#include "AgreementState.h"
#include "FlashingState.h"

BaseState* BaseState::forcedNextState = 0;

BaseState* BaseState::logicStep() {
  if (digitalRead(PIN_SWITCH) == HIGH) {
    // The switch is off, we want to get to off
    return &ShutDownState::INSTANCE;
  }

  if(forcedNextState) {
    BaseState *tmp = forcedNextState;
    forcedNextState = 0;
    return tmp;
  }

  // else, we do not care
  return 0;
};

void BaseState::sendCurrentSettings() {
  logger.println( F("Sending current settings") );
  size_t data_size = sizeof(settings)+1;
  uint8_t data[data_size];
  data[0] = '$';
  memcpy(data+1, &settings, data_size-1);
  packetSerial.send(data, data_size);
  Serial.flush();
}

bool BaseState::processCommand(const uint8_t* buffer, size_t size) {
  if (size == 0)
    return false;
  logger.println( F("Processing command") );
  unsigned char c = buffer[0];
  switch (c) {
    case '#':
      // TODO: reimplement here
      return true;
    case 'a':
      logger.println( F("Forced switch to AgreementState") );
      forcedNextState = &AgreementState::INSTANCE;
      return true;
      break;
    case '$':
      if(size != sizeof(settings)+1) {
          logger.println( F("Invalid settings message") );
          // we have received the wrong amount of bytes. Ignore this message and throw an error
          sendCommand("E1",2);
          return true;
        } else {
          // We know the buffer has the correct size, cast it
          struct settings *received_settings = (struct settings*)(buffer+1);

          // Check the CRC
          uint16_t crc = CRC16.ccitt((const uint8_t *) received_settings, sizeof(struct settings)-2);
          if(crc != received_settings->crcChecksum) {
            logger.println( F("Rx'ed settings CRC error") );
            // CRC error, notify receiver and ignore
            sendCommand("E2", 2);
            return true;
          }

          // check if settings are valid
          if(!checkSettings(received_settings)) {
            logger.println( F("Rx'ed settings invalid") );
            sendCommand("E3", 2);
            return true;
          }

          // CRC is OK. Set dirty by comparing the new settings to the old ones and overwrite the settings if changed.
          if(0 == memcmp(&settings, received_settings, sizeof(struct settings))) {
            // Settings are the same, don't do anything.
            logger.println( F("Rx'ed settings are not new") );
          } else {
            // We have new settings. Set the dirty flag and overwrite settings
            logger.println( F("Rx'ed settings are new") );
            settingsDirty = true;
            settings = *received_settings;
            updateSettingDependencies();
          }

          // Send a k to notify setting reception success
          sendCommand('k');
      }
      return true;
      break;
    case 'f':
      logger.println( F("Forced switch to FlashingState") );
      forcedNextState = &FlashingState::INSTANCE;
      sendCommand('F');
      return true;
      break;
    case '!':
      if(size == 2) {
        heartbeatDeactivated = buffer[1] == 0;
        lastHeartbeat = millis();
        logger.println( F("Set heartbeat state") );
      }
      break;
    case '?':
      logger.println( F("Rx'ed request for current settings") );
      sendCurrentSettings();
      break;
    case '<':
      logger.println( F("Shift LED offset -") );
      if(settings.ledOffset == 0) {
        settings.ledOffset = settings.ledCount;
      } else {
        settings.ledOffset--;
      }
      settings.crcChecksum = CRC16.ccitt((const uint8_t *) &settings, sizeof(settings)-2);
      settingsDirty = true;
      sendCurrentSettings();
      showCenterLed(400);
      break;
    case '>':
      logger.println( F("Shift LED offset +") );
      settings.ledOffset = (settings.ledOffset+1)%settings.ledCount;
      settings.crcChecksum = CRC16.ccitt((const uint8_t *) &settings, sizeof(settings)-2);
      settingsDirty = true;
      sendCurrentSettings();
      showCenterLed(400);
      break;
    default:
      return false;
  }
  return true;
};

void BaseState::blink(uint8_t count) {
  while (count-- > 0) {
    digitalWrite(PIN_STATUS, HIGH);
    delay(100);
    digitalWrite(PIN_STATUS, LOW);
    delay(100);
  }
}

void BaseState::readSettings() {
  logger.println( F("Reading settings") );
  EEPROM.get(0, settings);
  settingsDirty = false;

  // Check, if CRC is OK.
  uint16_t crc = CRC16.ccitt((const uint8_t *) &settings, sizeof(settings) - 2);

  if (crc == settings.crcChecksum) {
    logger.println( F("Settings CRC valid -> apply") );
    updateSettingDependencies();
    return;
  }

  // load defaults
  logger.println( F("Settings CRC invalid -> apply defaults") );
  settings = default_settings;
  // calculate crc for default settings
  settings.crcChecksum = CRC16.ccitt((const uint8_t *) &settings, sizeof(settings) - 2);
  updateSettingDependencies();
}

void BaseState::writeSettings() {
  if(!settingsDirty)
  {
    logger.println( F("Settings not dirty, skip write") );
    return;
  }

  logger.println( F("Settings dirty, calc CRC & write") );
  settings.crcChecksum = CRC16.ccitt((const uint8_t *) &settings, sizeof(settings)-2);
  EEPROM.put(0, settings);
  settingsDirty = false;
}

// Updates the dependencies (e.g. ring) using the new settings
void BaseState::updateSettingDependencies() {
  ring.updateLength(settings.ledCount);
  ring.updateType(supportedPixels[settings.ledTypeIndex]);
  ring.setBrightness(settings.maxLedBrightness);
}

void BaseState::enter() {
  millisEnteredState = millis();
}

unsigned long BaseState::timeInState() {
  return millis() - millisEnteredState;
}

void BaseState::sendCommand(const char command) {
  sendCommand(&command, 1);
}

void BaseState::sendCommand(const char *command, uint8_t length) {
  logger.print( F("Sending cmd with len ") );
  logger.println(length);
  packetSerial.send((const uint8_t*)command, length);
  Serial.flush();
}

bool BaseState::checkSettings(struct settings* settings_ptr) {
  if(settings_ptr->ledTypeIndex >= PIXEL_TYPES)
    return false;

  if(settings_ptr->ledCount > 32)
    return false;

  if(settings_ptr->countDownMillis > 15000)
    return false;

  if(settings_ptr->flashDurationMicros > 1000000)
    return false;
  return true;
}


void BaseState::showCenterLed(uint16_t duration) {
  for(int8_t i=0; i<ring.numPixels(); i++) {
    if(i == ring.numPixels()-1) {
      ring.setPixelColor((i+settings.ledOffset)%ring.numPixels(), 0x00FF00);
    } else {
      ring.setPixelColor((i+settings.ledOffset)%ring.numPixels(), 0x000000);
    }
  }
  ring.show();
  delay(duration);
  for(int8_t i=0; i<ring.numPixels(); i++) {
    ring.setPixelColor(i, 0x000000);
  }
  ring.show();
}
