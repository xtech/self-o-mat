//
// Created by clemens on 12.02.19.
//

#ifndef SELF_O_MAT_IMAGEPROCESSOR_H
#define SELF_O_MAT_IMAGEPROCESSOR_H

#include <Magick++.h>
#include <iostream>
#include "ILogger.h"
#include "imageinfo.h"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

extern "C" {
    #include <turbojpeg.h>
}

using namespace Magick;
class ImageProcessor {
private:
    tjhandle tj;
    int scalingFactorCount = 0;
    tjscalingfactor *scalingFactors = nullptr;

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
    Image frameImageForPrint(void *inputImage, ImageInfo imageInfo);
    bool convertImageToPreview(Image &image, void **outBuffer, size_t *outBufferSize, ImageInfo *outImageInfo);

    bool start();
    bool stop();

    virtual ~ImageProcessor();
};


#endif //SELF_O_MAT_IMAGEPROCESSOR_H
