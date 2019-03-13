//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_IMAGEPROCESSOR_H
#define SELF_O_MAT_IMAGEPROCESSOR_H


#include <Magick++.h>
#include <magick/profile.h>
#include <iostream>
#include "ILogger.h"
#include "imageinfo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "verbose.h"
#include "JpegDecoder.h"

using namespace Magick;
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

    bool start();
    bool stop();

    virtual ~ImageProcessor();
};


#endif //SELF_O_MAT_IMAGEPROCESSOR_H
