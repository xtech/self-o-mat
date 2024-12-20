//
// Created by clemens on 24.09.19.
//

#include "verbose.h"
#include <spdlog/spdlog.h>
using namespace selfomat::tools;

void selfomat::tools::LOG_D(std::string tag, std::string message, std::string arg) {
    spdlog::debug("[{}]: {}{}", tag, message, arg);
}

void selfomat::tools::LOG_I(std::string tag, std::string message, std::string arg) {
    spdlog::info("[{}]: {}{}", tag, message, arg);
}

void selfomat::tools::LOG_W(std::string tag, std::string message, std::string arg) {
    spdlog::warn("[{}]: {}{}", tag, message, arg);
}

void selfomat::tools::LOG_E(std::string tag, std::string message, std::string arg) {
    spdlog::error("[{}]: {}{}", tag, message, arg);
}
