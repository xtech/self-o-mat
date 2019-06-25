//
// Created by clemens on 12.02.19.
//


#include "ImageProcessor.h"

using namespace selfomat::logic;
using namespace selfomat::tools;

const std::string ImageProcessor::TAG = "IMAGE PROCESSOR";

bool ImageProcessor::start() {
    loadTemplateImage();
    return true;
}

void ImageProcessor::loadTemplateImage() {
    LOG_D(TAG, "Trying to open template image");
    cv::Mat templateImage = cv::imread(std::string(getenv("HOME")) + "/.template.png", -1);

    if(templateImage.cols == 0 || templateImage.rows == 0 || templateImage.channels() != 4) {
        alphaChannel = cv::Mat();
        templateWithoutAlpha = cv::Mat();
        return;
    }


    // extract alpha channel from template
    cv::extractChannel(templateImage, alphaChannel, 3);
    alphaChannel.convertTo(alphaChannel, CV_32FC1);
    alphaChannel /= 255.0f;
    cv::cvtColor(templateImage, templateWithoutAlpha, cv::COLOR_BGRA2BGR);


    // Read properties for the template
    if (isTemplateLoaded()) {
        boost::property_tree::ptree ptree;
        try {
            boost::property_tree::read_json(std::string(getenv("HOME")) + "/.template_props.json", ptree);
            offset = cv::Rect(ptree.get<int>("offset_x"),  ptree.get<int>("offset_y"),
                              ptree.get<int>("offset_w"), ptree.get<int>("offset_h"));
        } catch (boost::exception &e) {
            logger->logError(std::string("Error loading template properties"));
            alphaChannel = cv::Mat();
            templateWithoutAlpha = cv::Mat();
        }
    }

}

ImageProcessor::ImageProcessor(ILogger *logger) {
    if (logger != nullptr) {
        this->logger = logger;
    } else {
        this->logger = new ConsoleLogger();
    }
}

ImageProcessor::~ImageProcessor() {

}


cv::Mat
ImageProcessor::frameImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter, double filterGain) {

    if (!isTemplateLoaded())
        return decodeImageForPrint(inputImageJpeg, jpegBufferSize, filter, filterGain);

    struct timespec tstart, tend;


    int targetHeight = offset.height;
    int targetWidth = offset.width;


    clock_gettime(CLOCK_MONOTONIC, &tstart);


    // Decode using Turbojpeg
    ImageInfo latestImageInfo{};
    jpegDecoder.decodeJpeg((unsigned char *) inputImageJpeg, jpegBufferSize, &latestBuffer, &latestBufferSize,
                           &latestImageInfo, BGR, targetWidth, targetHeight, LARGER_THAN_REQUIRED);


    cv::Mat decodedImage(latestImageInfo.height, latestImageInfo.width, CV_8UC3, latestBuffer);

    int imageWidth = decodedImage.cols;
    int imageHeight = decodedImage.rows;


    // Create the resulting image it has to be as large as the print template
    cv::Mat result(templateWithoutAlpha.rows, templateWithoutAlpha.cols, CV_8UC3, cv::Scalar(255,255,255));

    // Get the ROI from the source image with the correct aspect ratio
    float templateAspectRatio = (float) targetWidth / (float) targetHeight;
    float imageAspectRatio = (float) imageWidth / (float) imageHeight;
    cv::Mat decodedImageROI;
    if (templateAspectRatio > imageAspectRatio) {
        // The template is wider than the original image
        int width = imageWidth;
        int height = imageWidth * (1.0f / templateAspectRatio);
        int xOffset = 0;
        int yOffset = (imageHeight - height) / 2;
        decodedImageROI = cv::Mat(decodedImage, cv::Rect(xOffset, yOffset, width, height));
    } else {
        // The template is wider than the original image
        int width = imageHeight * templateAspectRatio;
        int height = imageHeight;
        int xOffset = (imageWidth - width) / 2;
        int yOffset = 0;
        decodedImageROI = cv::Mat(decodedImage, cv::Rect(xOffset, yOffset, width, height));
    }


    cv::Size targetSize(targetWidth, targetHeight);
    cv::Mat resized;
    cv::resize(decodedImageROI, resized, targetSize);
    resized.copyTo(result(offset));


    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("scale took %.5f s\n",
           ((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
           ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));
    clock_gettime(CLOCK_MONOTONIC, &tstart);
    applyFilter(decodedImage, filter, filterGain);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("filtering took %.5f s\n",
           ((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
           ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));

    clock_gettime(CLOCK_MONOTONIC, &tstart);

    auto alphaPtr = reinterpret_cast<float *>(alphaChannel.data);
    auto imagePtr = reinterpret_cast<cv::Vec3b *>(result.data);
    auto templatePtr = reinterpret_cast<cv::Vec3b *>(templateWithoutAlpha.data);

    unsigned int amountPixels = result.rows * result.cols;

    while(amountPixels-->0) {
        *imagePtr = (1.0f-*alphaPtr) * *imagePtr + *alphaPtr * *templatePtr;
        templatePtr++;
        alphaPtr++;
        imagePtr++;
    }


    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("composition took %.5f s\n",
           ((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
           ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));



    return result;
}

bool ImageProcessor::stop() {
    return true;
}

cv::Mat
ImageProcessor::decodeImageForPrint(void *inputImageJpeg, size_t jpegBufferSize, FILTER filter, double filterGain) {
    struct timespec tstart, tend;


    int targetHeight = 1182;
    int targetWidth = 1773;

    clock_gettime(CLOCK_MONOTONIC, &tstart);


    // Decode using Turbojpeg
    ImageInfo latestImageInfo{};
    jpegDecoder.decodeJpeg((unsigned char *) inputImageJpeg, jpegBufferSize, &latestBuffer, &latestBufferSize,
                           &latestImageInfo, BGR, targetWidth, targetHeight, LARGER_THAN_REQUIRED);


    cv::Mat decodedImage(latestImageInfo.height, latestImageInfo.width, CV_8UC3, latestBuffer);


    clock_gettime(CLOCK_MONOTONIC, &tstart);

    {
        float widthFactor = (float)targetWidth / (float)decodedImage.cols;
        float heightFactor = (float)targetHeight / (float)decodedImage.rows;
        float scalingFactor = std::max(widthFactor, heightFactor);
        LOG_D(TAG, "scaling with: " << widthFactor << ", " << heightFactor << ", " << scalingFactor);
        cv::resize(decodedImage, decodedImage,
                   cv::Size(decodedImage.cols * scalingFactor, decodedImage.rows * scalingFactor));
    }

    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("scaling took %.5f s\n",
           ((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
           ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));


    clock_gettime(CLOCK_MONOTONIC, &tstart);
    applyFilter(decodedImage, filter, filterGain);
    clock_gettime(CLOCK_MONOTONIC, &tend);
    printf("filtering took %.5f s\n",
           ((double) tend.tv_sec + 1.0e-9 * tend.tv_nsec) -
           ((double) tstart.tv_sec + 1.0e-9 * tstart.tv_nsec));


    return decodedImage;
}

bool ImageProcessor::getOffset(cv::Mat image, cv::Rect &out) {
    // Check if image has alpha channel
    if (image.channels() != 4) {
        return false;
    }


    // extract alpha channel.
    cv::Mat alpha;
    cv::extractChannel(image, alpha, 3);

    int left = -1, right = -1, top = -1, bottom = -1;
    bool transparencyFound = false;
    for (unsigned int row = 0; row < alpha.rows; row++) {
        for (unsigned int col = 0; col < alpha.cols; col++) {
            bool isTransparent = alpha.at<uchar>(row, col) < 255;
            if(isTransparent) {
                if(left < 0 || left > col)
                    left = col;
                if(right < 0 || right < col)
                    right = col;
                if(top < 0 || top > row)
                    top = row;
                if(bottom < 0 || bottom < row)
                    bottom = row;
                transparencyFound = true;
            }
        }
    }

    if(!transparencyFound)
        return false;

    out.x = left;
    out.y = top;
    out.width = right - left;
    out.height = bottom - top;

    std::cout << "x: " << out.x << ", y: " << out.y << ", width: " << out.width << ", height: "
              << out.height << std::endl;

    return true;
}

void ImageProcessor::writeOffset(cv::Rect offset, std::string filename) {
    boost::property_tree::ptree ptree;
    ptree.put("offset_x", offset.x);
    ptree.put("offset_y", offset.y);
    ptree.put("offset_w", offset.width);
    ptree.put("offset_h", offset.height);

    boost::property_tree::write_json(filename, ptree);
}

bool ImageProcessor::updateTemplate(void *data, size_t size) {
    cv::Mat blob(1, size, CV_8UC1, data);
    cv::Mat image = cv::imdecode(blob, -1);
    cv::Mat imagePrint;
    cv::Mat imageScreen;

    {
        // TODO: Remove Hardcoded Size
        float widthFactor = 1864.0f / image.cols;
        float heightFactor = 1228.0f / image.rows;
        float factor = std::fmax(widthFactor, heightFactor);
        cv::resize(image, imagePrint, cv::Size(image.cols * factor, image.rows * factor));
    }
    {
        // TODO: Remove Hardcoded Size
        float widthFactor = 1280.0f / image.cols;
        float heightFactor = 800.0f / image.rows;
        float factor = std::fmax(widthFactor, heightFactor);
        cv::resize(image, imageScreen, cv::Size(image.cols * factor, image.rows * factor));
    }

    cv::Rect offsetPrint;
    cv::Rect offsetScreen;

    if(!getOffset(imagePrint, offsetPrint)) {
        // No alpha found, so the image isn't a proper template
        return false;
    }


    if (!getOffset(imageScreen, offsetScreen)) {
        return false;
    }

    std::string home = std::string(getenv("HOME"));
    std::string imageFilename = home + "/.template.png";
    std::string imageScreenFilename = home + "/.template_screen.png";
    std::string offsetFilename = home + "/.template_props.json";
    std::string offsetScreenFilename = home + "/.template_screen_props.json";

    try {
        cv::imwrite(imageFilename, imagePrint);
        cv::imwrite(imageScreenFilename, imageScreen);
        writeOffset(offsetPrint, offsetFilename);
        writeOffset(offsetScreen, offsetScreenFilename);
    } catch (std::exception &error) {
        // Clean up if there was an error
        std::remove(imageFilename.c_str());
        std::remove(imageScreenFilename.c_str());
        std::remove(offsetFilename.c_str());
        std::remove(offsetScreenFilename.c_str());
        alphaChannel = cv::Mat();
        templateWithoutAlpha = cv::Mat();
        return false;
    }

    loadTemplateImage();

    return true;
}

void ImageProcessor::applyFilter(cv::Mat &image, FILTER filter, double gain) {
    cv::imwrite("before_filter.jpg", image);
    switch (filter) {
        case BASIC_FILTER:
            LOG_D(TAG, "basic filter with gain: " << gain);
            basicFilter.processImage(image, gain);
            break;
        default:
            LOG_D(TAG, "No Filter");
            break;
    }
    cv::imwrite("after_filter.jpg", image);
}

const std::vector<std::string> *ImageProcessor::getFilterNames() {
    return &filterNames;
}
