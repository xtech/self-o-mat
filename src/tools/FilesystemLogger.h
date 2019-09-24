//
// Created by clemens on 24.09.19.
//

#ifndef SELF_O_MAT_FILESYSTEMLOGGER_H
#define SELF_O_MAT_FILESYSTEMLOGGER_H

#include <boost/filesystem/fstream.hpp>
#include <boost/thread/recursive_mutex.hpp>
#include <iostream>
#include <tools/ILogger.h>

namespace selfomat {
    namespace tools {
        class FilesystemLogger {
        public:

            static FilesystemLogger INSTANCE;
            enum LOG_TO_FILE_STATE {
                // Disabled if we don't want to log to file
                        DISABLED,
                // Error if the logging is enabled but there was an error writing the file
                        ERROR,
                // Enabled if the logging is currently active
                        ENABLED
            };

            LOG_TO_FILE_STATE getLogToFileState();

            LOG_TO_FILE_STATE enableLogToFile(std::string filename);

            LOG_TO_FILE_STATE disableLogToFile();

            void logToFile(std::string s);

            void registerDelegate(ILogger *delegate);

        private:
            FilesystemLogger();

            ILogger *loggerDelegate = nullptr;

            boost::recursive_mutex fileLogMutex;

            boost::filesystem::ofstream ofstream;

            LOG_TO_FILE_STATE logToFileState;
        };
    }
}



#endif //SELF_O_MAT_FILESYSTEMLOGGER_H
