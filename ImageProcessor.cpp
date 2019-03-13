//
// Created by clemens on 12.02.19.
//


#include "ImageProcessor.h"
#include "ConsoleLogger.h"
#include "buffers.h"

const std::string ImageProcessor::TAG = "IMAGE PROCESSOR";

bool ImageProcessor::start() {
    LOG_D(TAG, "Trying to open template image");
    try {
        templateImage.read("./assets/template.png");
    } catch (Exception &error_) {
        logger->logError(std::string("Error loading template image: ") + error_.what());
        return false;
    }

    // Read properties for the template
    boost::property_tree::ptree ptree;
    try {
        boost::property_tree::read_json("./assets/template_props.json", ptree);
        offsetTop = ptree.get<int>("offset_top");
        offsetLeft = ptree.get<int>("offset_left");
        offsetRight = ptree.get<int>("offset_right");
        offsetBottom = ptree.get<int>("offset_bottom");
    } catch (Exception &e) {
        logger->logError(std::string("Error loading template properties: ") + e.what());
        return false;
    }


    return true;
}

ImageProcessor::ImageProcessor(ILogger *logger) : templateImage() {
    if (logger != nullptr) {
        this->logger = logger;
    } else {
        this->logger = new ConsoleLogger();
    }
}

ImageProcessor::~ImageProcessor() {

}


Image ImageProcessor::frameImageForPrint(void *inputImageJpeg, size_t jpegBufferSize) {
    struct timespec tstart, tend;
    int targetHeight = offsetBottom - offsetTop;
    int targetWidth = offsetRight - offsetLeft;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    ImageInfo latestResult{};
    auto success = jpegDecoder.decodeJpeg((unsigned char *) inputImageJpeg, jpegBufferSize,
                                          &latestBuffer, &latestBufferSize, &latestResult, RGB,
                                          static_cast<unsigned int>(targetWidth),
                                          static_cast<unsigned int>(targetHeight), LARGER_THAN_REQUIRED);

    // Create imagemagick image from blob
    Image inputImageMagic(latestResult.width, latestResult.height, "RGB", StorageType::CharPixel, latestBuffer);

    int imageWidth = latestResult.width;
    int imageHeight = latestResult.height;

    LOG_D(TAG, "We decoded an image with size " << imageWidth << "x" << imageHeight);

    int targetCenterX = (offsetRight - offsetLeft) / 2.0 + offsetLeft;
    int targetCenterY = (offsetBottom - offsetTop) / 2.0 + offsetTop;


    float scaleY = (float) targetHeight / (float) imageHeight;
    float scaleX = (float) targetWidth / (float) imageWidth;

    float scaleToUse = fmax(scaleX, scaleY);


    int centerX = (float) scaleToUse*imageWidth / 2.0;
    int centerY = (float) scaleToUse*imageHeight / 2.0;

    LOG_D(TAG, "We need to scale with: " << scaleToUse);

    // Create the resulting image it has to be as large as the print template
    Image result(templateImage);
    inputImageMagic.scale(Geometry(ceil(imageWidth * scaleToUse), ceil(imageHeight * scaleToUse)));
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("scale took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    clock_gettime(CLOCK_MONOTONIC, &tstart);

    result.composite(inputImageMagic, targetCenterX - centerX, targetCenterY - centerY, DstOverCompositeOp);

    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("composition took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    return result;
}

bool ImageProcessor::stop() {
    return true;
}


