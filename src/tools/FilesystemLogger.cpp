//
// Created by clemens on 24.09.19.
//

#include "FilesystemLogger.h"


using namespace selfomat::tools;


FilesystemLogger FilesystemLogger::INSTANCE;

FilesystemLogger::LOG_TO_FILE_STATE FilesystemLogger::getLogToFileState() {
    return logToFileState;
}

FilesystemLogger::LOG_TO_FILE_STATE FilesystemLogger::enableLogToFile(std::string filename) {
    boost::unique_lock<boost::recursive_mutex> lk(fileLogMutex);
    if(ofstream.is_open()) {
        disableLogToFile();
    }
    ofstream.open(boost::filesystem::path{filename+"/debug.log"}, boost::filesystem::ofstream::out | boost::filesystem::ofstream::app);
    if(ofstream.is_open())
        logToFileState = ENABLED;
    else
        logToFileState = ERROR;
    return logToFileState;
}

FilesystemLogger::LOG_TO_FILE_STATE FilesystemLogger::disableLogToFile() {
    boost::unique_lock<boost::recursive_mutex> lk(fileLogMutex);
    if(ofstream.is_open())
        ofstream.close();
    logToFileState = DISABLED;
    return logToFileState;}

FilesystemLogger::FilesystemLogger() : fileLogMutex() {
    logToFileState = DISABLED;
}

void FilesystemLogger::logToFile(std::string s) {
    fileLogMutex.lock();
    if(logToFileState == ENABLED) {
        ofstream << s << std::endl;
        ofstream.flush();
    }
    if(loggerDelegate != nullptr) {
        loggerDelegate->log(s);
    }
    fileLogMutex.unlock();
}

void FilesystemLogger::registerDelegate(ILogger *delegate) {
    loggerDelegate = delegate;
}

