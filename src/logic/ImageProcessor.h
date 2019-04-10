//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_IMAGEPROCESSOR_H
#define SELF_O_MAT_IMAGEPROCESSOR_H


#include <Magick++.h>
#include <magick/profile.h>
#include <iostream>
#include <tools/ILogger.h>
#include <tools/imageinfo.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <tools/ConsoleLogger.h>
#include <tools/buffers.h>
#include <tools/verbose.h>
#include <tools/JpegDecoder.h>
#include <easyexif/exif.h>

using namespace Magick;
using namespace selfomat::tools;

namespace selfomat {
    namespace logic {
        class ImageProcessor {
        private:
            static const std::string TAG;

            Blob adobeRgbIcc;
            Blob sRgbIcc;

            JpegDecoder jpegDecoder;

            Image templateImage;
            int offsetTop;
            int offsetLeft;
            int offsetBottom;
            int offsetRight;

            ILogger *logger;

            void *latestBuffer = nullptr;
            size_t latestBufferSize = 0;

        public:
            explicit ImageProcessor(ILogger *logger);

            Image frameImageForPrint(void *inputImageJpeg, size_t jpegBufferSize);
            Image decodeImageForPrint(void *inputImageJpeg, size_t jpegBufferSize);

            bool start();

            bool stop();

            virtual ~ImageProcessor();
        };
    }
}

#endif //SELF_O_MAT_IMAGEPROCESSOR_H
