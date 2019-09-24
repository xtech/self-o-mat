//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_ILOGGER_H
#define SELF_O_MAT_ILOGGER_H

#include <string>
#include <iostream>



namespace selfomat {
    namespace tools {
        class ILogger {

        public:
            virtual void log(std::string s) = 0;

            virtual ~ILogger() = default;
        };
    }
}

#endif //SELF_O_MAT_ILOGGER_H
