//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_CONSOLELOGGER_H
#define SELF_O_MAT_CONSOLELOGGER_H


#include "tools/ILogger.h"
#include <iostream>

using namespace selfomat::tools;

namespace selfomat {
    namespace tools {
        class ConsoleLogger : public ILogger {
        public:

        protected:
            void log(std::string s) override;
        };
    }
}

#endif //SELF_O_MAT_CONSOLELOGGER_H
