//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_IMAGEPROCESSOR_H
#define SELF_O_MAT_IMAGEPROCESSOR_H


#include <iostream>
#include <tools/imageinfo.h>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <tools/buffers.h>
#include <tools/verbose.h>
#include <tools/JpegDecoder.h>
#include <opencv2/opencv.hpp>
#include "logic/filters/BasicImageFilter.h"
#include "logic/filters/GrayscaleImageFilter.h"


using namespace selfomat::tools;

namespace selfomat {
    namespace logic {

        enum FILTER {
            NO_FILTER = 0,
            BASIC_FILTER = 1,
            GRAYSCALE_FILTER = 2
        };

        const std::vector<std::string> filterNames { "No Filter", "Basic Filter", "Grayscale Filter" };

        class ImageProcessor {
        private:

            static const std::string TAG;

            JpegDecoder jpegDecoder;

            cv::Rect offset;

            void *latestBuffer = nullptr;
            size_t latestBufferSize = 0;

            bool getOffset(cv::Mat image, cv::Rect &out);
            void writeOffset(cv::Rect offset, std::string filename);

            BasicImageFilter basicFilter;
            GrayscaleImageFilter grayscaleFilter;

            void applyFilter(cv::Mat &image, FILTER filter, double gain);

            cv::Mat alphaChannel;
            cv::Mat templateWithoutAlpha;

            void loadTemplateImage();

        public:
            explicit ImageProcessor();

            cv::Mat frameImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter = NO_FILTER, double filterGain = 1.0);
            cv::Mat decodeImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter = NO_FILTER, double filterGain = 1.0);

            bool start();

            bool stop();

            virtual ~ImageProcessor();

            bool isTemplateLoaded() {
                return templateWithoutAlpha.cols > 0 && templateWithoutAlpha.rows > 0;
            }

            bool updateTemplate(void *data, size_t size);

            const std::vector<std::string> * getFilterNames();
        };
    }
}

#endif //SELF_O_MAT_IMAGEPROCESSOR_H
