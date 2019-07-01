//
// Created by clemens on 27.05.19.
//


#include "SelfomatController.h"

using namespace selfomat::logic;

const SelfomatController::LED_TYPE SelfomatController::LED_TYPE::RGB = SelfomatController::LED_TYPE("RGB",0);
const SelfomatController::LED_TYPE SelfomatController::LED_TYPE::RGBW = SelfomatController::LED_TYPE("RGBW",1);

const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_12 = SelfomatController::LED_COUNT("12 LEDs",12);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_16 = SelfomatController::LED_COUNT("16 LEDs",16);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_24 = SelfomatController::LED_COUNT("24 LEDs",24);
const SelfomatController::LED_COUNT SelfomatController::LED_COUNT::COUNT_32 = SelfomatController::LED_COUNT("32 LEDs",32);



bool SelfomatController::autoconnect(std::string searchPrefix) {
    std::cout << "[selfomat controller] Searching controller. Prefix: " << searchPrefix << std::endl;

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
        std::cerr << "[selfomat controller] No controller paths found :-(" << std::endl;
        return false;
    }

    // We have found at least one port
    bool foundController = false;
    boost::filesystem::path controllerPath;
    for(const auto &path : foundArduinos) {
        std::cout << "[selfomat controller] Connecting to controller at: " << path << std::endl;

        try {
            tmpSerialPort.open(path.string());
            tmpSerialPort.set_option(boost::asio::serial_port_base::baud_rate(38400));

            std::cout << "[selfomat controller] Waiting for identification" << std::endl;

            blocking_reader reader(tmpSerialPort, 3000);
            bool success = false;
            for(int i = 0; i < 10; i++) {
                if (reader.get_response("\2i ", 3, c)) {
                    std::cout << "[selfomat controller] Got a " << c << std::endl;
                    if (c == 'b') {
                        std::cout << "[selfomat controller] Found the selfomat controller!" << std::endl;
                        tmpSerialPort.close();
                        foundController = true;
                        controllerPath = path;
                        break;
                    }
                    std::cout << "[selfomat controller] Unknown identification: " << c << std::endl;
                } else {
                    std::cout << "[selfomat controller] No identification received" << std::endl;
                }
            }
            if(foundController)
                break;
            tmpSerialPort.close();
        } catch (std::exception const &e) {
            std::cerr << "[selfomat controller] Error opening button on port " << path << ". Reason was: " << e.what() << std::endl;
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
                std::cout << "[selfomat controller] setting loading timed out. retrying.." << std::endl;
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
                std::cout << "[selfomat controller] setting writing timed out. retrying.." << std::endl;
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
        std::cerr << "[selfomat controller] Error opening button on port " << path << ". Reason was: " << e.what() << std::endl;
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
                std::cout << "[selfomat controller] controller saved settings successfully" << std::endl;
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
                        std::cerr << "[selfomat controller] CRC ERROR! was: " << received_settings->crcChecksum << ", expected: " << crc << std::endl;
                    }
                } else {
                    std::cerr << "[selfomat controller] wrong setting size received. We got: " << commandSequence.size() << ": but expected " << sizeof(struct settings)+1 << std::endl;
                }
            }
            break;
        case 'E': {
                char arg = '-';
                if(commandSequence.size() > 1)
                    arg = commandSequence[1];

                boost::unique_lock<boost::mutex> lk(state_mutex);
                if(arg != '3') {
                    std::cerr << "[selfomat controller] Controller had a problem receiving settings. Argument was: " << arg << ". Retrying"
                              << std::endl;
                    settingsWritten = false;
                } else {
                    std::cerr << "[selfomat controller] Error writing setting. Board says values are invalid :-(" << std::endl;
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
            if(logic != nullptr)
                logic->cancelPrint();
            break;
        case 't':
            if(logic != nullptr)
                logic->trigger();
            break;
        case 'd':
            if(logic != nullptr)
                logic->stop();
            break;
        default:
            std::cout << "[selfomat controller] got unknown command from button. Type was: " << ((char)commandType) << std::endl;
            break;
    }
}

void SelfomatController::enterUpdateMode() {
    if(!isConnected)
        return;
    {
        boost::unique_lock<boost::mutex> lk(state_mutex);
        flashStateEntered = false;

        do {
            sendCommand('f');
            if(!state_condition_variable.timed_wait(lk, boost::posix_time::milliseconds(500))) {
                std::cout << "update mode timed out. retrying.." << std::endl;
            }

        } while (!flashStateEntered && isStarted);
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
                std::cout << "agreement timed out. retrying.." << std::endl;
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

void SelfomatController::stopBlocking() {
    isStarted = false;
    io_service.stop();
    if(controllerThreadHandle.joinable())
        controllerThreadHandle.join();
    if(button_serial_port.is_open())
        button_serial_port.close();
}

void SelfomatController::ioThread() {
    std::cout << "[selfomat controller] Controller Thread Started" << std::endl;
    while(isStarted) {
        boost::asio::io_service::work work(io_service);
        io_service.run();
    }
    std::cout << "[selfomat controller] Controller Thread stopped" << std::endl;
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

