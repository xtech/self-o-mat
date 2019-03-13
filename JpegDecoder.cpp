//
// Created by clemens on 13.03.19.
//

#include "JpegDecoder.h"

JpegDecoder::JpegDecoder() {
    std::cout << "new jpeg decoder" << std::endl;
    tj = tjInitDecompress();
    scalingFactors = tjGetScalingFactors(&scalingFactorCount);

    if (scalingFactorCount == 0) {
        std::cerr << "Jpeg Decoder error: No scaling factors available." << std::endl;
    }
}

JpegDecoder::~JpegDecoder() {
    std::cout << "jpeg decoder destroyed" << std::endl;
    tjDestroy(tj);
}

bool JpegDecoder::decodeJpeg(unsigned char *jpeg_data, size_t jpeg_size, void **outBuffer, size_t *outBufferSize,
                             ImageInfo *resultInfo, JPEG_DECODE_SCALE_TYPE scale_type) {
    return false;
}

