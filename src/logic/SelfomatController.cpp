//
// Created by clemens on 27.05.19.
//


#include "SelfomatController.h"

using namespace selfomat::logic;
using namespace selfomat::tools;

const SelfomatController::LED_TYPE SelfomatController::LED_TYPE::RGB = SelfomatController::LED_TYPE("RGB",0);
const SelfomatController::LED_TYPE SelfomatController::LED_TYPE::RGBW = SelfomatController::LED_TYPE("RGBW",1);

const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_12 = SelfomatController::LED_COUNT("12 LEDs",12);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_16 = SelfomatController::LED_COUNT("16 LEDs",16);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_24 = SelfomatController::LED_COUNT("24 LEDs",24);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_32 = SelfomatController::LED_COUNT("32 LEDs",32);

std::string SelfomatController::TAG = "SELFOMAT_CONTROLLER";

bool SelfomatController::autoconnect(std::string searchPrefix) {
    LOG_D(TAG, "Searching controller. Prefix: ", searchPrefix);

    boost::asio::io_service tmpIoService;
    boost::asio::serial_port tmpSerialPort(tmpIoService);
    char c = 0;

    // First, search paths for potential controller connections
    boost::filesystem::path devPath("/dev/");

    std::vector<boost::filesystem::path> foundArduinos;
    for (auto &e : boost::make_iterator_range(boost::filesystem::directory_iterator(devPath))) {
        if (boost::starts_with(e.path().string(), searchPrefix))
            foundArduinos.push_back(e.path());
    }

    // We have not found a controller
    if(foundArduinos.empty()) {
        LOG_E(TAG, "No controller paths found :-(");
        return false;
    }

    // We have found at least one port
    bool foundController = false;
    boost::filesystem::path controllerPath;
    for(const auto &path : foundArduinos) {
        LOG_D(TAG, "Connecting to controller at: ", path.string());

        try {
            tmpSerialPort.open(path.string());
            tmpSerialPort.set_option(boost::asio::serial_port_base::baud_rate(38400));

            LOG_D(TAG, "Waiting for identification");

            blocking_reader reader(tmpSerialPort, 3000);
            bool success = false;
            for(int i = 0; i < 10; i++) {
                if (reader.get_response("\2i ", 3, c)) {
                    LOG_D(TAG, "Got a ",  std::to_string(c));
                    if (c == 'b') {
                        LOG_D(TAG, "Found the selfomat controller!");
                        tmpSerialPort.close();
                        foundController = true;
                        controllerPath = path;
                        break;
                    }
                    LOG_W(TAG, "Unknown identification: ");
                } else {
                    LOG_W(TAG, "No identification received");
                }
            }
            if(foundController)
                break;
            tmpSerialPort.close();
        } catch (std::exception const &e) {
            LOG_E(TAG, std::string("Error opening button on port ") + path.string() + ". Reason was: " + e.what());
        }
    }

    if(!foundController)
        return false;


    // We found the controller, connect for real
    return connect(controllerPath);
}

SelfomatController::SelfomatController() : io_service(), button_serial_port(io_service) {
    this->logic = nullptr;
}

void SelfomatController::setLogic(ILogicController *logic) {
    this->logic = logic;
}

bool SelfomatController::loadSettingsFromBoard() {
    if(!isConnected)
        return false;
    {
        boost::unique_lock<boost::mutex> lk(state_mutex);
        settingsLoaded = false;

        do {
            sendCommand('?');
            if(!state_condition_variable.timed_wait(lk, boost::posix_time::milliseconds(1500))) {
                LOG_I(TAG, "setting loading timed out. retrying..");
            }

        } while (!settingsLoaded && isStarted);
    }
    return true;
}

bool SelfomatController::writeSettingsToBoard() {
    if(!isConnected)
        return false;
    {
        boost::unique_lock<boost::mutex> lk(state_mutex);
        // fix CRC before sending
        crc16.reset();
        crc16.process_bytes(&settings, sizeof(struct settings) - 2);
        uint16_t crc = crc16.checksum();
        settings.crcChecksum = crc;

        cobs::ByteSequence command;
        command.push_back('$');
        uint8_t *settings_ptr = (uint8_t *)(&settings);
        command.insert(command.end(), settings_ptr, settings_ptr+sizeof(struct settings));
        cobs::ByteSequence encoded = cobs::cobs_encode(command);
        encoded.push_back(' ');

        settingsWritten = false;

        do {
            button_serial_port.write_some(boost::asio::buffer(encoded, encoded.size()));
            if(!state_condition_variable.timed_wait(lk, boost::posix_time::milliseconds(500))) {
                LOG_I(TAG, "setting writing timed out. retrying..");
            }

        } while (!settingsWritten && isStarted);
    }
    return true;
}

bool SelfomatController::connect(boost::filesystem::path path) {
    try {
        button_serial_port.open(path.string());
        button_serial_port.set_option(boost::asio::serial_port_base::baud_rate(38400));
    } catch (std::exception const &e) {
        LOG_E(TAG, std::string("Error opening button on port ") + path.string() + ". Reason was: " + e.what());
        return false;
    }

    isConnected = true;
    isStarted = true;
    // Start the controller thread
    controllerThreadHandle = boost::thread(boost::bind(&SelfomatController::ioThread, this));


    // Register callback for received characters
    registerSerialPacketCallback();

    // We're connected. Read the settings and send a heartbeat
    sendHeartbeat();
    loadSettingsFromBoard();

    return true;
}

void SelfomatController::sendHeartbeat() {
    sendCommand('.');
}


void SelfomatController::setWatchdogEnabled(bool enabled) {
    if(!isConnected)
        return;
    cobs::ByteSequence command;
    command.push_back('!');
    command.push_back(enabled);

    cobs::ByteSequence encoded = cobs::cobs_encode(command);
    encoded.push_back(' ');
    button_serial_port.write_some(boost::asio::buffer(encoded, encoded.size()));
}

void SelfomatController::sendPictureTaken() {
    sendCommand('k');
}

void SelfomatController::triggerFlash() {
    sendCommand('#');
}

void SelfomatController::setLedType(uint8_t ledType) {
    settings.ledTypeIndex = ledType;
}

void SelfomatController::setLedCount(uint8_t ledCount) {
    if(ledCount < 12)
        ledCount = 12;
    settings.ledCount = ledCount;
}

void SelfomatController::setCountDownMillis(uint16_t countDownMillis) {
    settings.countDownMillis = countDownMillis;
}

void SelfomatController::setFlashDurationMicros(uint32_t flashDurationMicros) {
    settings.flashDurationMicros = flashDurationMicros;
}

void SelfomatController::handleCommand(cobs::ByteSequence &commandSequence) {

    if (commandSequence.empty())
        return;

    uint8_t commandType = commandSequence[0];
    switch (commandType) {
        case 'k': {
                boost::unique_lock<boost::mutex> lk(state_mutex);
                LOG_I(TAG, "controller saved settings successfully");
                settingsWritten = true;
                state_condition_variable.notify_all();
            }
            break;
        case '$': {
                if (commandSequence.size() == 1 + sizeof(struct settings)) {
                    // we have the correct size. Check CRC
                    struct settings *received_settings = (struct settings *) (commandSequence.data() + 1);

                    crc16.reset();
                    crc16.process_bytes(received_settings, sizeof(struct settings) - 2);
                    uint16_t crc = crc16.checksum();
                    if (crc == received_settings->crcChecksum) {
                        boost::unique_lock<boost::mutex> lk(state_mutex);

                        settings = *received_settings;
                        settingsLoaded = true;
                        state_condition_variable.notify_all();
                    } else {
                        LOG_E(TAG, std::string("CRC ERROR! was: ") + std::to_string(received_settings->crcChecksum) + ", expected: " + std::to_string(crc) );
                    }
                } else {
                    LOG_E(TAG, std::string("wrong setting size received. We got: ") + std::to_string(commandSequence.size()) + ": but expected " + std::to_string(sizeof(struct settings)+1));
                }
            }
            break;
        case 'E': {
                char arg = '-';
                if(commandSequence.size() > 1)
                    arg = commandSequence[1];

                boost::unique_lock<boost::mutex> lk(state_mutex);
                if(arg != '3') {
                    LOG_E(TAG, std::string("Controller had a problem receiving settings. Argument was: ") + arg + ". Retrying");
                    settingsWritten = false;
                } else {
                    LOG_E(TAG, "Error writing setting. Board says values are invalid :-(");
                    settingsWritten = true;
                }
                state_condition_variable.notify_all();
            }
            break;
        case 'A': {
                boost::unique_lock<boost::mutex> lk(state_mutex);
                agreementStateEntered = true;
                state_condition_variable.notify_all();
            }
            break;
        case 'F': {
            boost::unique_lock<boost::mutex> lk(state_mutex);
            flashStateEntered = true;
            state_condition_variable.notify_all();
        }
            break;
        case 'a':
            if(logic != nullptr)
                logic->acceptAgreement();
            break;
        case 'c':
            if(logic != nullptr) {
                // the SelfomatController alone only know that a button has been
                // pressed which is related to printing; it depends on a setting
                // if that shall be interpreted as "cancel" or "confirm"
                if( logic->getPrintConfirmationEnabled() ) {
                    LOG_I(TAG, "confirming print");
                    logic->confirmPrint();
                }
                else {
                    LOG_I(TAG, "canceling print");
                    logic->cancelPrint();
                }
            }
            break;
        case 't':
            if(logic != nullptr)
                logic->trigger();
            break;
        case 'd':
            stop(false);

            if(logic != nullptr) {
                logic->stop();
            }
            break;
        default:
            LOG_W(TAG, "[selfomat controller] got unknown command from button. Type was: ", std::to_string(((char)commandType)));
            break;
    }
}

void SelfomatController::enterUpdateMode() {
    if(!isConnected)
        return;
    LOG_I(TAG, "Entering Update Mode...");
    {
        boost::unique_lock<boost::mutex> lk(state_mutex);
        flashStateEntered = false;

        do {
            sendCommand('f');
            if(!state_condition_variable.timed_wait(lk, boost::posix_time::milliseconds(500))) {
                LOG_E(TAG, "update mode timed out. retrying..");
            }

        } while (!flashStateEntered && isStarted);

        LOG_I(TAG, "Update Mode Confirmed!");
    }
}

void SelfomatController::showPrinting() {
    sendCommand('p');
}

void SelfomatController::showAgreement() {
    if(!isConnected)
        return;
    {
        boost::unique_lock<boost::mutex> lk(state_mutex);
        agreementStateEntered = false;

        do {
            sendCommand('a');
            if(!state_condition_variable.timed_wait(lk, boost::posix_time::milliseconds(500))) {
                LOG_W(TAG, "agreement timed out. retrying..");
            }

        } while (!agreementStateEntered && isStarted);
    }
}

SelfomatController::~SelfomatController() {

}

void SelfomatController::setStressTestEnabled(bool enabled) {
    if(enabled) {
        sendCommand('S');
    } else {
        sendCommand('s');
    }
}

void SelfomatController::remoteTrigger() {
    if (!isConnected) {
        logic->trigger();
    } else {
        sendCommand('t');
    }
}

uint8_t SelfomatController::getLedType() {
    return settings.ledTypeIndex;
}

uint8_t SelfomatController::getLedCount() {
    return settings.ledCount;
}

uint16_t SelfomatController::getCountDownMillis() {
    return settings.countDownMillis;
}

uint32_t SelfomatController::getFlashDurationMicros() {
    return settings.flashDurationMicros;
}

void SelfomatController::stop(bool blocking) {
    if (!isStarted)
        return;

    LOG_I(TAG, "Controller Thread stopping...");

    isStarted = false;
    io_service.stop();

    if(blocking && controllerThreadHandle.joinable())
        controllerThreadHandle.join();
    if(button_serial_port.is_open())
        button_serial_port.close();
}

void SelfomatController::stopBlocking() {
    stop(true);
}

void SelfomatController::ioThread() {
    LOG_I(TAG, "Controller Thread Started");
    while(isStarted) {
        boost::asio::io_service::work work(io_service);
        io_service.run();
    }
    LOG_I(TAG, "Controller Thread stopped");
}

void SelfomatController::serialPacketReceived(const boost::system::error_code &err, std::size_t bytes_transferred) {
    if(bytes_transferred > 1) {
        cobs::ByteSequence encoded(bytes_transferred-1);
        buffer_copy(boost::asio::buffer(encoded), serialBuffer.data());
        serialBuffer.consume(bytes_transferred);

        cobs::ByteSequence decoded = cobs::cobs_decode(encoded);
        handleCommand(decoded);
    } else {
        // just consume the space
        serialBuffer.consume(1);
    }

    registerSerialPacketCallback();
}

void SelfomatController::registerSerialPacketCallback() {
    boost::asio::async_read_until(button_serial_port, serialBuffer, " ", boost::bind(&SelfomatController::serialPacketReceived, this, boost::asio::placeholders::error,
                                                                                     boost::asio::placeholders::bytes_transferred));
}

bool SelfomatController::sendCommand(char command) {
    if(!button_serial_port.is_open())
        return false;
    // cobs encode the command
    cobs::ByteSequence data;
    data.push_back(command);
    cobs::ByteSequence encoded = cobs::cobs_encode(data);
    encoded.push_back(' ');
    button_serial_port.write_some(boost::asio::buffer(encoded, encoded.size()));
    return true;
}

void SelfomatController::commit() {
    writeSettingsToBoard();
}

void SelfomatController::moveOffsetRight() {
    sendCommand('>');
}

void SelfomatController::moveOffsetLeft() {
    sendCommand('<');
}

bool SelfomatController::getFlashMode() {
    return settings.flashMode > 0;
}

void SelfomatController::setFlashMode(bool ttlEnabled) {
    settings.flashMode = ttlEnabled;
}

void SelfomatController::setMaxLedBrightness(uint8_t maxBrightness) {
    settings.maxLedBrightness = maxBrightness;
}

uint8_t SelfomatController::getMaxLedBrightness() {
    return settings.maxLedBrightness;
}
