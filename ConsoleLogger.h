//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_CONSOLELOGGER_H
#define SELF_O_MAT_CONSOLELOGGER_H


#include "ILogger.h"
#include <iostream>

class ConsoleLogger : public ILogger {
public:
    void logDebug(std::string s) override;

    void logDebug(const char *s) override;

    void logInfo(std::string s) override;

    void logInfo(const char *s) override;

    void logWarn(std::string s) override;

    void logWarn(const char *s) override;

    void logError(std::string s) override;

    void logError(const char *s) override;
};


#endif //SELF_O_MAT_CONSOLELOGGER_H
