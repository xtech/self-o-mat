//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_VERBOSE_H
#define SELF_O_MAT_VERBOSE_H

#include <iostream>
#include <tools/FilesystemLogger.h>


namespace selfomat {
    namespace tools {
        void LOG_D(std::string tag, std::string message, std::string arg = "");

        void LOG_I(std::string tag, std::string message, std::string arg = "");

        void LOG_W(std::string tag, std::string message, std::string arg = "");

        void LOG_E(std::string tag, std::string message, std::string arg = "");
    }
}


#endif //SELF_O_MAT_VERBOSE_H
