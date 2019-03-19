//
// Created by clemens on 12.02.19.
//

#include "ConsoleLogger.h"

using namespace selfomat::tools;

void ConsoleLogger::logDebug(std::string s) {
    std::cout << "[Debug] " << s << std::endl;
}

void ConsoleLogger::logDebug(const char *s) {
    logDebug(std::string(s));
}

void ConsoleLogger::logInfo(std::string s) {
    std::cout << "[Info] " << s << std::endl;
}

void ConsoleLogger::logInfo(const char *s) {
    logInfo(std::string(s));
}

void ConsoleLogger::logWarn(std::string s) {
    std::cout << "[Warn] " << s << std::endl;
}

void ConsoleLogger::logWarn(const char *s) {
    logWarn(std::string(s));
}

void ConsoleLogger::logError(std::string s) {
    std::cerr << "[Error] " << s << std::endl;
}

void ConsoleLogger::logError(const char *s) {
    logError(std::string(s));
}
