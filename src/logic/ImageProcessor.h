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
#include <opencv2/opencv.hpp>
#include "BasicImageFilter.h"


using namespace Magick;
using namespace selfomat::tools;

namespace selfomat {
    namespace logic {

        enum FILTER {
            NO_FILTER = 0,
            BASIC_FILTER = 1
        };

        class ImageProcessor {
        private:
            struct Rect {
                int top;
                int right;
                int bottom;
                int left;
            };

            static const std::string TAG;

            Blob adobeRgbIcc;
            Blob sRgbIcc;

            JpegDecoder jpegDecoder;

            bool templateLoaded = false;
            Image templateImage;
            Rect offset;

            ILogger *logger;

            void *latestBuffer = nullptr;
            size_t latestBufferSize = 0;

            Rect getOffset(Image *image, int accuracy = 1);
            void writeOffset(Rect offset, std::string filename);

            BasicImageFilter basicFilter;

            void applyFilter(Image image, FILTER filter);

        public:
            explicit ImageProcessor(ILogger *logger);

            Image frameImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter = NO_FILTER);
            Image decodeImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter = NO_FILTER);

            bool start();

            bool stop();

            virtual ~ImageProcessor();

            bool isTemplateLoaded() {
                return templateLoaded;
            }

            bool updateTemplate(void *data, size_t size);



        };
    }
}

#endif //SELF_O_MAT_IMAGEPROCESSOR_H
