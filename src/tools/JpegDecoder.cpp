//
// Created by clemens on 13.03.19.
//

#include "JpegDecoder.h"

using namespace selfomat::tools;

const std::string JpegDecoder::TAG = "JPEG DECODER";

JpegDecoder::JpegDecoder() {
    LOG_D(TAG, "New Instance");
    tj = tjInitDecompress();
    scalingFactors = tjGetScalingFactors(&scalingFactorCount);

    if (scalingFactorCount == 0) {
        LOG_E(TAG, "Jpeg Decoder error: No scaling factors available.");
    }
}

JpegDecoder::~JpegDecoder() {
    LOG_D(TAG, "Destroyed");

    tjDestroy(tj);
}

bool
JpegDecoder::decodeJpegNoResize(unsigned char *jpegData, size_t jpegSize, void **outBuffer, size_t *outBufferSize,
                                ImageInfo *resultInfo, JPEG_DECODE_COLORS colors) {
    int jpegWidth, jpegHeight, depth;
    TJPF pixel_format;
    tjDecompressHeader(tj, jpegData, jpegSize, &jpegWidth, &jpegHeight);

    switch (colors) {
        case RGB:
            depth = 3;
            pixel_format = TJPF_RGB;
            break;
        case RGBA:
            depth = 4;
            pixel_format = TJPF_RGBA;
            break;
        default:
            LOG_E(TAG, "Unknown color format.");
            return false;
    }

    // Allocate the buffer
    auto sizeNeeded = static_cast<size_t>(depth * jpegWidth * jpegHeight);
    requireBufferWithSize(outBuffer, outBufferSize, sizeNeeded);

    // Decompress into the buffer
    auto retVal = tjDecompress2(tj, jpegData, jpegSize, (unsigned char *) *outBuffer, 0, 0, 0, pixel_format,
                                TJFLAG_NOREALLOC);

    if (retVal < 0) {
        LOG_E(TAG, "Error converting image, error was: " << tjGetErrorStr());
        return false;
    }


    resultInfo->width = static_cast<uint32_t>(jpegWidth);
    resultInfo->height = static_cast<uint32_t>(jpegHeight);

    return true;
}

bool JpegDecoder::decodeJpeg(unsigned char *jpegData, size_t jpegSize, void **outBuffer, size_t *outBufferSize,
                             ImageInfo *resultInfo, JPEG_DECODE_COLORS colors, unsigned int targetWidth,
                             unsigned int targetHeight, JPEG_DECODE_SCALE_TYPE scaleType) {

    if (0 == targetWidth && 0 == targetHeight) {
        // We don't want to scale
        return decodeJpegNoResize(jpegData, jpegSize, outBuffer, outBufferSize, resultInfo, colors);
    }



    // We need to scale
    int jpegWidth, jpegHeight, depth;
    TJPF pixel_format;
    tjDecompressHeader(tj, jpegData, jpegSize, &jpegWidth, &jpegHeight);

    switch (colors) {
        case BGR:
            depth = 3;
            pixel_format = TJPF_BGR;
            break;
        case RGB:
            depth = 3;
            pixel_format = TJPF_RGB;
            break;
        case RGBA:
            depth = 4;
            pixel_format = TJPF_RGBA;
            break;
        case BGRA:
            depth = 4;
            pixel_format = TJPF_BGRA;
            break;
        default:
            LOG_E(TAG, "Unknown color format.");
            return false;
    }


    // Check, if we need to scale. We only scale down so in order to scale both dimensions must be larger than the target
    if (jpegHeight > targetHeight && jpegWidth > targetWidth) {

        int scalingFactorIdx = -1;

        // Find the scaling factor
        switch (scaleType) {
            case LARGER_THAN_REQUIRED: {
                int bestDist = std::max(jpegWidth, jpegHeight);
                for (int i = 0; i < scalingFactorCount; i++) {
                    int scaledHeight = TJSCALED(jpegHeight, scalingFactors[i]);
                    int scaledWidth = TJSCALED(jpegWidth, scalingFactors[i]);
                    if (scaledHeight >= targetHeight && scaledWidth >= targetHeight) {
                        int dist = std::min(scaledHeight - targetHeight, scaledWidth - targetWidth);
                        if (dist < bestDist) {
                            scalingFactorIdx = i;
                            bestDist = dist;
                        }
                    }
                }
            }
                break;
            case SMALLER_THAN_REQUIRED: {
                int bestDist = std::max(jpegWidth, jpegHeight);
                for (int i = 0; i < scalingFactorCount; i++) {
                    int scaledHeight = TJSCALED(jpegHeight, scalingFactors[i]);
                    int scaledWidth = TJSCALED(jpegWidth, scalingFactors[i]);
                    if (scaledHeight <= targetHeight && scaledWidth <= targetHeight) {
                        int dist = std::min(targetHeight - scaledHeight, targetWidth - scaledWidth);
                        if (dist < bestDist) {
                            scalingFactorIdx = i;
                            bestDist = dist;
                        }
                    }
                }
            }
                break;
            default:
                LOG_E(TAG, "Unknown scale type provided");
                return false;
        }

        if (scalingFactorIdx < 0) {
            LOG_E(TAG, "We could not find a scaling for the requested image. This is a bug, please report it");
            return false;
        }

        LOG_D(TAG, "Using scaling for decoding image with size: " << jpegWidth << "x" << jpegHeight << std::endl
                                                                  << "Scaling used: "
                                                                  << scalingFactors[scalingFactorIdx].num << "/"
                                                                  << scalingFactors[scalingFactorIdx].denom);


        tjscalingfactor factor = scalingFactors[scalingFactorIdx];

        // Allocate the buffer
        auto sizeNeeded = static_cast<size_t>(depth * TJSCALED(jpegWidth, factor) * TJSCALED(jpegHeight, factor));

        requireBufferWithSize(outBuffer, outBufferSize, sizeNeeded);

        auto retVal = tjDecompress2(tj, jpegData, jpegSize, (unsigned char *) *outBuffer,
                                    TJSCALED(jpegWidth, factor), 0, TJSCALED(jpegHeight, factor),
                                    pixel_format, TJFLAG_NOREALLOC);
        if (retVal < 0) {
            LOG_E(TAG, "Error converting image, error was: " << tjGetErrorStr());
            return false;
        }

        resultInfo->width = static_cast<uint32_t >(TJSCALED(jpegWidth, factor));
        resultInfo->height = static_cast<uint32_t >(TJSCALED(jpegHeight, factor));

        return true;
    } else {
        // Decode without scaling. The image is already smaller than the requested size.
        // Allocate the buffer
        auto sizeNeeded = static_cast<size_t>(depth * jpegWidth * jpegHeight);
        requireBufferWithSize(outBuffer, outBufferSize, sizeNeeded);

        // Decompress into the buffer
        auto retVal = tjDecompress2(tj, jpegData, jpegSize, (unsigned char *) *outBuffer, 0, 0, 0, pixel_format,
                                    TJFLAG_NOREALLOC);
        if (retVal < 0) {
            LOG_E(TAG, "Error converting image, error was: " << tjGetErrorStr());
            return false;
        }


        resultInfo->width = static_cast<uint32_t>(jpegWidth);
        resultInfo->height = static_cast<uint32_t>(jpegHeight);

        return true;
    }
}



