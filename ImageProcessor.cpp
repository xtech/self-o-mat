//
// Created by clemens on 12.02.19.
//


#include "ImageProcessor.h"
#include "ConsoleLogger.h"
#include "buffers.h"


bool ImageProcessor::start() {
    tj = tjInitDecompress();
    scalingFactors = tjGetScalingFactors(&scalingFactorCount);


    std::cout << "Trying to open template image" << std::endl;
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
    // Decode the jpeg
    int jpegWidth, jpegHeight;
    tjDecompressHeader(tj, (unsigned char *) inputImageJpeg, jpegBufferSize, &jpegWidth, &jpegHeight);

    int bestDist = 99999999;
    int scalingFactorIdx = -1;

    for (int i = 0; i < scalingFactorCount; i++) {
        int scaledHeight = TJSCALED(jpegHeight, scalingFactors[i]);
        if (scaledHeight >= targetHeight) {
            int dist = scaledHeight - targetHeight;
            if (dist < bestDist) {
                scalingFactorIdx = i;
                bestDist = dist;
            }
        }
    }

    tjscalingfactor factor = scalingFactors[scalingFactorIdx];
    std::cout << "We will use scaling of: " << factor.num << "/" << factor.denom << std::endl;
    std::cout << "Scaled size: " << TJSCALED(jpegWidth, factor) << "x" << TJSCALED(jpegHeight, factor) << std::endl;



    // Allocate the buffer
    auto sizeNeeded = static_cast<size_t>(4 * TJSCALED(jpegWidth, factor) * TJSCALED(jpegHeight, factor));

    // Temp buffer for huge image
    //void *tmpBuf = NULL;
    //size_t tmpSize = 0;
    buffers::requireBufferWithSize(&latestBuffer, &latestBufferSize, sizeNeeded);


    auto retVal = tjDecompress2(tj, (unsigned char *) inputImageJpeg, jpegBufferSize,
                                (unsigned char *) latestBuffer, 0, 0, targetHeight,
                                TJPF_RGBA, TJFLAG_NOREALLOC);

    // Create imagemagick image from blob
    Image inputImageMagic(TJSCALED(jpegWidth, factor), TJSCALED(jpegHeight, factor), "RGBA", StorageType::CharPixel, latestBuffer);

    int imageWidth = inputImageMagic.columns();
    int imageHeight = inputImageMagic.rows();

    std::cout << "We decoded an image with size " << imageWidth << "x" << imageHeight << std::endl;


    int targetCenterX = (offsetRight - offsetLeft) / 2.0 + offsetLeft;
    int targetCenterY = (offsetBottom - offsetTop) / 2.0 + offsetTop;


    float scaleY = (float) targetHeight / (float) imageHeight;
    float scaleX = (float) targetWidth / (float) imageWidth;

    float scaleToUse = fmax(scaleX, scaleY);


    int centerX = (float) scaleToUse*imageWidth / 2.0;
    int centerY = (float) scaleToUse*imageHeight / 2.0;

    std::cout << "We need to scale with: " << scaleToUse << std::endl;

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


Image ImageProcessor::frameImageForPrint(void *inputImage, ImageInfo imageInfo) {
    struct timespec tstart, tend;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    // Create imagemagick image from blob
    Image inputImageMagic(imageInfo.width, imageInfo.height, "RGBA", StorageType::CharPixel, inputImage);

    int imageWidth = inputImageMagic.columns();
    int imageHeight = inputImageMagic.rows();

    std::cout << "We decoded an image with size " << imageWidth << "x" << imageHeight << std::endl;

    int targetHeight = offsetBottom - offsetTop;
    int targetWidth = offsetRight - offsetLeft;

    int targetCenterX = (offsetRight - offsetLeft) / 2.0 + offsetLeft;
    int targetCenterY = (offsetBottom - offsetTop) / 2.0 + offsetTop;


    float scaleY = (float) targetHeight / (float) imageHeight;
    float scaleX = (float) targetWidth / (float) imageWidth;

    float scaleToUse = fmax(scaleX, scaleY);


    int centerX = (float) scaleToUse*imageWidth / 2.0;
    int centerY = (float) scaleToUse*imageHeight / 2.0;

    std::cout << "We need to scale with: " << scaleToUse << std::endl;

    // Create the resulting image it has to be as large as the print template
    Image result(templateImage);
    inputImageMagic.scale(Geometry(ceil(imageWidth * scaleToUse), ceil(imageHeight * scaleToUse)));

    result.composite(inputImageMagic, targetCenterX - centerX, targetCenterY - centerY, DstOverCompositeOp);

    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("composition took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    return result;
}

bool ImageProcessor::convertImageToPreview(Image &image, void **outBuffer, size_t *outBufferSize, ImageInfo *outImageInfo) {
    struct timespec tstart, tend;

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    size_t resultSize = image.columns() * image.rows() * 4;
    buffers::requireBufferWithSize(outBuffer, outBufferSize, resultSize);
    image.write(0, 0, image.columns(), image.rows(), "RGBA", Magick::CharPixel, *outBuffer);

    outImageInfo->width = image.columns();
    outImageInfo->height = image.rows();
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("conversion took %.5f s\n",
           ((double)tend.tv_sec + 1.0e-9*tend.tv_nsec) -
           ((double)tstart.tv_sec + 1.0e-9*tstart.tv_nsec));

    return true;
}

bool ImageProcessor::stop() {
    tjDestroy(tj);
    return true;
}


