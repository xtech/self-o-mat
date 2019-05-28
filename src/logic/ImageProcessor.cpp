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
        templateImage.read(std::string(getenv("HOME")) + "/.template.png");
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
            boost::property_tree::read_json(std::string(getenv("HOME")) + "/.template_props.json", ptree);
            Rect offset = (Rect){
                    ptree.get<int>("offset_top"),
                    ptree.get<int>("offset_right"),
                    ptree.get<int>("offset_bottom"),
                    ptree.get<int>("offset_left")
            };

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



    int targetHeight = offset.bottom - offset.top;
    int targetWidth = offset.right - offset.left;

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

    int targetCenterX = targetWidth / 2.0 + offset.left;
    int targetCenterY = targetHeight / 2.0 + offset.top;


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



    int targetHeight = 1182;
    int targetWidth = 1773;

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

    return inputImageMagic;
}

ImageProcessor::Rect ImageProcessor::getOffset(Image image, bool isScreen) {

    int width = (int) image.columns();
    int height = (int) image.rows();

    Rect offset = (Rect){0, width, height, 0};

    bool foundTop = false;
    bool foundLeft = false;
    bool foundBottom = false;
    bool foundRight = false;

    while (offset.top < offset.bottom && offset.left < offset.right) {

        for (int x=offset.left; x<offset.right; x++) {

            if (foundTop || image.getConstPixels(x, offset.top, 1, 1)->opacity == 0) {
                foundTop = true;
            }
            if (foundBottom || image.getConstPixels(x, offset.bottom, 1, 1)->opacity == 0) {
                foundBottom = true;
            }
            if (foundBottom && foundTop) {
                break;
            }
        }

        for (int y=offset.top; y<offset.bottom; y++) {

            if (foundLeft || image.getConstPixels(offset.left, y, 1, 1)->opacity == 0) {
                foundLeft = true;
            }
            if (foundRight || image.getConstPixels(offset.right, y, 1, 1)->opacity == 0) {
                foundRight = true;
            }
            if (foundLeft && foundRight) {
                break;
            }
        }

        if (!foundTop) offset.top++;
        if (!foundBottom) offset.bottom--;
        if (!foundLeft) offset.left++;
        if (!foundRight) offset.right--;

        if (foundTop && foundBottom && foundLeft && foundRight) {
            break;
        }
    }

    if (!foundTop || !foundBottom || !foundLeft || !foundRight) {
        offset = (Rect){0, width, height, 0};
    } else {
        // Ensure that the area between template and image is 100% filled  with either template or the image, so zoom the image a little bit bigger.
        offset = (Rect){
            (int)fmax(0, offset.top-2),
            (int)fmin(width, offset.right+2),
            (int)fmin(height, offset.bottom+2),
            (int)fmax(0, offset.left-2)
        };
    }

    std::cout << "Top: " << offset.top << ", Right: " << offset.right << ", Bottom: " << offset.bottom << ", Left: " << offset.left << std::endl;

    boost::property_tree::ptree ptree;
    ptree.put("offset_top", offset.top);
    ptree.put("offset_right", offset.right);
    ptree.put("offset_bottom", offset.bottom);
    ptree.put("offset_left", offset.left);

    std::string filename;
    if (isScreen) {
        filename = "/.template_screen_props.json";
    } else {
        filename = "/.template_props.json";
    }

    try {
        boost::property_tree::write_json(std::string(getenv("HOME")) + filename, ptree);
    } catch (boost::exception &e) {
        std::cerr << "Error writing template props." << std::endl;
    }

    return offset;
}

void ImageProcessor::updateTemplate(void *data, size_t size) {
    Magick::Blob blob = Blob(data, size);
    Image image = Image(blob);

    image.channel(DefaultChannels);
    image.alphaChannel(ActivateAlphaChannel);

    image.backgroundColor(Color());

    image.resize(Geometry("1864x1228^"));
    image.extent(Geometry("1864x1228"), CenterGravity);

    image.write(std::string(getenv("HOME")) + "/.template.png");

    this->offset = getOffset(image, false);
    this->templateImage = image;
    this->templateLoaded = true;

    Image imageScreen = Image(image);

    imageScreen.thumbnail(Geometry("1280x800^"));
    imageScreen.extent(Geometry("1280x800"), CenterGravity);

    imageScreen.write(std::string(getenv("HOME")) + "/.template_screen.png");

    getOffset(imageScreen, true);
}