//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_JPEGDECODER_H
#define SELF_O_MAT_JPEGDECODER_H


#include <turbojpeg.h>
#include <iostream>
#include "imageinfo.h"

extern "C" {
    #include <turbojpeg.h>
}

/**
 * The scaling type for decoding the image.
 */
enum JPEG_DECODE_SCALE_TYPE {
    LARGER_THAN_REQUIRED,
    SMALLER_THAN_REQUIRED
};

enum JPEG_DECODE_COLORS {
    RGB,
    RGBA
};

class JpegDecoder {
private:
    tjhandle tj;
    int scalingFactorCount = 0;
    tjscalingfactor *scalingFactors = nullptr;


public:
    JpegDecoder();

    /**
     * Decodes a jpeg image to a ARGB bitmap
     * @param jpeg_data
     * @param jpeg_size
     * @param outBuffer
     * @param outBufferSize
     * @param resultInfo
     * @param scale_type
     * @return
     */
    bool decodeJpeg(unsigned char *jpeg_data, size_t jpeg_size, void **outBuffer, size_t *outBufferSize,
                    ImageInfo *resultInfo, JPEG_DECODE_SCALE_TYPE scale_type);

    virtual ~JpegDecoder();
};


#endif //SELF_O_MAT_JPEGDECODER_H
