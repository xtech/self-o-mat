//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_ILOGGER_H
#define SELF_O_MAT_ILOGGER_H

#include <string>

class ILogger {
public:
    virtual void logDebug(std::string s) = 0;

    virtual void logDebug(const char *s) = 0;

    virtual void logInfo(std::string s) = 0;

    virtual void logInfo(const char *s) = 0;

    virtual void logWarn(std::string s) = 0;

    virtual void logWarn(const char *s) = 0;

    virtual void logError(std::string s) = 0;

    virtual void logError(const char *s) = 0;

    virtual ~ILogger() {

    }
};

#endif //SELF_O_MAT_ILOGGER_H
