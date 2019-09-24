//
// Created by clemens on 24.09.19.
//

#include "verbose.h"

using namespace selfomat::tools;

void selfomat::tools::LOG_D(std::string tag, std::string message, std::string arg) {
    bool logToFile = FilesystemLogger::INSTANCE.getLogToFileState() == FilesystemLogger::ENABLED;
    std::string file_info_str;
    if(!logToFile) {
        file_info_str = "!";
    } else {
        file_info_str = ">";
    }
    std::string s = "[DBG][" + tag + "]: " + message+arg;
    std::cout << file_info_str << s << std::endl;
    FilesystemLogger::INSTANCE.logToFile(s);
}

void selfomat::tools::LOG_I(std::string tag, std::string message, std::string arg) {
    bool logToFile = FilesystemLogger::INSTANCE.getLogToFileState() == FilesystemLogger::ENABLED;
    std::string file_info_str;
    if(!logToFile) {
        file_info_str = "!";
    } else {
        file_info_str = ">";
    }
    std::string s = "[INF][" + tag + "]: " + message+arg;
    std::cout << file_info_str << s << std::endl;
    FilesystemLogger::INSTANCE.logToFile(s);
}

void selfomat::tools::LOG_W(std::string tag, std::string message, std::string arg) {
    bool logToFile = FilesystemLogger::INSTANCE.getLogToFileState() == FilesystemLogger::ENABLED;
    std::string file_info_str;
    if(!logToFile) {
        file_info_str = "!";
    } else {
        file_info_str = ">";
    }
    std::string s = "[WRN][" + tag + "]: " + message+arg;
    std::cout << file_info_str << s << std::endl;
    FilesystemLogger::INSTANCE.logToFile(s);
}

void selfomat::tools::LOG_E(std::string tag, std::string message, std::string arg) {
    bool logToFile = FilesystemLogger::INSTANCE.getLogToFileState() == FilesystemLogger::ENABLED;
    std::string file_info_str;
    if(!logToFile) {
        file_info_str = "!";
    } else {
        file_info_str = ">";
    }
    std::string s = "[ERR][" + tag + "]: " + message+arg;
    std::cout << file_info_str << s << std::endl;
    FilesystemLogger::INSTANCE.logToFile(s);
}
