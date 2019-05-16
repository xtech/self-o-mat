//
// Created by clemens on 12.02.19.
//


#include "ImageProcessor.h"

using namespace selfomat::logic;
using namespace selfomat::tools;

const std::string ImageProcessor::TAG = "IMAGE PROCESSOR";

bool ImageProcessor::start() {
    LOG_D(TAG, "Trying to open template image");
    try {
        templateImage.read("/opt/assets/template.png");
        templateLoaded = true;
    } catch (Exception &error_) {
        templateLoaded = false;
        logger->logError(std::string("Error loading template image: ") + error_.what());
    }

    LOG_D(TAG, "Loading Adobe RGB Profile");
    try {
        Image magick;
        magick.read("./assets/AdobeRGB1998.icc");
        magick.write(&adobeRgbIcc);
        LOG_D(TAG, "Profile loaded. Blob size: " << adobeRgbIcc.length());
    } catch (Exception &error) {
        logger->logError(std::string("error loading icc profile: ") + error.what());
        return false;
    }
    LOG_D(TAG, "Loading sRGB Profile");
    try {
        Image magick;
        magick.read("./assets/sRGB2014.icc");
        magick.write(&sRgbIcc);
        LOG_D(TAG, "Profile loaded. Blob size: " << sRgbIcc.length());
    } catch (Exception &error) {
        logger->logError(std::string("error loading icc profile: ") + error.what());
        return false;
    }

    // Read properties for the template
    if(templateLoaded) {
        boost::property_tree::ptree ptree;
        try {
            boost::property_tree::read_json("/opt/assets/template_props.json", ptree);
            offsetTop = ptree.get<int>("offset_top");
            offsetLeft = ptree.get<int>("offset_left");
            offsetRight = ptree.get<int>("offset_right");
            offsetBottom = ptree.get<int>("offset_bottom");
        } catch (Exception &e) {
            logger->logError(std::string("Error loading template properties: ") + e.what());
            return false;
        }
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

    if(!templateLoaded)
        return decodeImageForPrint(inputImageJpeg, jpegBufferSize);

    struct timespec tstart, tend;



    int targetHeight = offsetBottom - offsetTop;
    int targetWidth = offsetRight - offsetLeft;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    bool srgb = true;

    // Parse exif information for the color profile used
    easyexif::EXIFInfo exifResult;
    int exifParseResultCode = exifResult.parseFrom((const unsigned char*)inputImageJpeg, static_cast<unsigned int>(jpegBufferSize));
    if(exifParseResultCode) {
        LOG_E(TAG, "Could not parse exif data. Assuming sRGB");
    } else {
        if(exifResult.ColorSpace == 2) {
            srgb = false;
        } else if(exifResult.ColorSpace == 65535 && exifResult.InteropIndex == "R03" ){
            srgb = false;
        } else if(exifResult.ColorSpace == 1) {
            srgb = true;
        } else if(exifResult.InteropIndex == "R98") {
            srgb = true;
        } else {
            LOG_E(TAG, "Could not determine sRGB or Adobe RGB. I'll assume sRGB.");
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("reading exif took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    clock_gettime(CLOCK_MONOTONIC, &tstart);


    // Decode using Turbojpeg
    ImageInfo latestImageInfo{};
    jpegDecoder.decodeJpeg((unsigned char*)inputImageJpeg, jpegBufferSize, &latestBuffer, &latestBufferSize,
            &latestImageInfo, RGB, targetWidth, targetHeight, LARGER_THAN_REQUIRED);


    Image inputImageMagic(latestImageInfo.width, latestImageInfo.height, "RGB", StorageType::CharPixel, latestBuffer);



    if(srgb) {
        LOG_D(TAG, "Decoding image as sRGB");
        inputImageMagic.iccColorProfile(sRgbIcc);
    } else {
        LOG_D(TAG, "Decoding image as Adobe RGB");
        inputImageMagic.iccColorProfile(adobeRgbIcc);
    }


    int imageWidth = inputImageMagic.columns();
    int imageHeight = inputImageMagic.rows();

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

    result.profile("ICC", sRgbIcc);

    return result;
}

bool ImageProcessor::stop() {
    return true;
}

Image ImageProcessor::decodeImageForPrint(void *inputImageJpeg, size_t jpegBufferSize) {
    struct timespec tstart, tend;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    bool srgb = true;

    // Parse exif information for the color profile used
    easyexif::EXIFInfo exifResult;
    int exifParseResultCode = exifResult.parseFrom((const unsigned char*)inputImageJpeg, static_cast<unsigned int>(jpegBufferSize));
    if(exifParseResultCode) {
        LOG_E(TAG, "Could not parse exif data. Assuming sRGB");
    } else {
        if(exifResult.ColorSpace == 2) {
            srgb = false;
        } else if(exifResult.ColorSpace == 65535 && exifResult.InteropIndex == "R03" ){
            srgb = false;
        } else if(exifResult.ColorSpace == 1) {
            srgb = true;
        } else if(exifResult.InteropIndex == "R98") {
            srgb = true;
        } else {
            LOG_E(TAG, "Could not determine sRGB or Adobe RGB. I'll assume sRGB.");
        }
    }


    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("reading exif took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));
    clock_gettime(CLOCK_MONOTONIC, &tstart);


    // Decode using Turbojpeg
    ImageInfo latestImageInfo{};
    jpegDecoder.decodeJpeg((unsigned char*)inputImageJpeg, jpegBufferSize, &latestBuffer, &latestBufferSize,
                           &latestImageInfo, RGB);


    Image inputImageMagic(latestImageInfo.width, latestImageInfo.height, "RGB", StorageType::CharPixel, latestBuffer);



    if(srgb) {
        LOG_D(TAG, "Decoding image as sRGB");
        inputImageMagic.iccColorProfile(sRgbIcc);
    } else {
        LOG_D(TAG, "Decoding image as Adobe RGB");
        inputImageMagic.iccColorProfile(adobeRgbIcc);
    }

    return inputImageMagic;
}


