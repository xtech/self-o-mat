//
// Created by clemens on 27.05.19.
//

#ifndef SELF_O_MAT_SELFOMATCONTROLLER_H
#define SELF_O_MAT_SELFOMATCONTROLLER_H

#include "ILogicController.h"
#include <stdint.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/serial_port.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <iostream>
#include <tools/cobs.h>
#include <tools/blocking_reader.h>
#include <boost/asio.hpp>
#include <boost/crc.hpp>



namespace selfomat {
    namespace logic {



        class SelfomatController {

        private:
#pragma pack(push, 1)
            struct settings {
                uint8_t ledTypeIndex;
                uint8_t ledCount;
                uint8_t ledOffset;
                uint16_t countDownMillis;
                uint32_t flashDurationMicros;
                uint16_t crcChecksum;
            } __attribute__((packed)) settings;
#pragma pack(pop)

        public:

            class LED_TYPE {
            public:
                static const LED_TYPE RGB;
                static const LED_TYPE RGBW;
                const std::string humanName;
                const uint8_t controllerValue;
            private:
                LED_TYPE(std::string humanName, uint8_t controllerValue): humanName(humanName), controllerValue(controllerValue) {}
            };

            class LED_COUNT {
            public:
                static const LED_COUNT COUNT_12;
                static const LED_COUNT COUNT_16;
                static const LED_COUNT COUNT_24;
                static const LED_COUNT COUNT_32;
                const std::string humanName;
                const uint8_t controllerValue;
            private:
                LED_COUNT(std::string humanName, uint8_t controllerValue): humanName(humanName), controllerValue(controllerValue) {}
            };

        private:
            ILogicController *logic;
            boost::crc_ccitt_type crc16;
            boost::asio::streambuf serialBuffer;
            boost::asio::io_service io_service;
            boost::asio::serial_port button_serial_port;
            boost::thread controllerThreadHandle;

            bool isConnected = false;
            bool isStarted = false;

            boost::condition_variable state_condition_variable;
            boost::mutex state_mutex;
            // BEGIN STATE
            struct settings currentSettings;
            // Initially we don't know the state of the button board
            bool settingsLoaded = false;
            bool settingsWritten = false;
            bool aggreementStateEntered = false;
            // END STATE

            bool loadSettingsFromBoard();

            bool writeSettingsToBoard();

            bool connect(boost::filesystem::path path);

            bool sendCommand(char command);

            void handleCommand(cobs::ByteSequence &commandSequence);

            void ioThread();

            void registerSerialPacketCallback();

            void serialPacketReceived(const boost::system::error_code &err, std::size_t bytes_transferred);

        public:
            SelfomatController();

            void setLogic(ILogicController *logic);

            bool autoconnect(std::string searchPrefix);

            virtual ~SelfomatController();

            void setStressTestEnabled(bool enabled);

            void enterUpdateMode();

            void showPrinting();

            void showAgreement();

            void remoteTrigger();

            void sendHeartbeat();

            void moveOffsetRight();

            void moveOffsetLeft();

            void setWatchdogEnabled(bool enabled);

            void sendPictureTaken();

            void triggerFlash();

            void setLedType(uint8_t ledType);

            uint8_t getLedType();

            void setLedCount(uint8_t ledCount);

            uint8_t getLedCount();

            void setCountDownMillis(uint16_t countDownMillis);

            uint16_t getCountDownMillis();

            void setFlashDurationMicros(uint32_t flashDurationMicros);

            uint32_t getFlashDurationMicros();

            void stopBlocking();

            void commit();

        };
    }
}

#endif //SELF_O_MAT_SELFOMATCONTROLLER_H
