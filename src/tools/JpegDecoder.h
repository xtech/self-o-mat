//
// Created by clemens on 13.03.19.
//

#ifndef SELF_O_MAT_JPEGDECODER_H
#define SELF_O_MAT_JPEGDECODER_H


#include <turbojpeg.h>
#include <iostream>
#include <algorithm>
#include "imageinfo.h"
#include "buffers.h"
#include "verbose.h"

extern "C" {
#include <turbojpeg.h>
}

namespace selfomat {
    namespace tools {
/**
 * The scaling type for decoding the image.
 */
        enum JPEG_DECODE_SCALE_TYPE {
            LARGER_THAN_REQUIRED,
            SMALLER_THAN_REQUIRED
        };

        enum JPEG_DECODE_COLORS {
            BGR,
            RGB,
            RGBA,
            BGRA
        };

        class JpegDecoder {
        private:
            static const std::string TAG;

            tjhandle tj;
            int scalingFactorCount = 0;
            tjscalingfactor *scalingFactors = nullptr;

            bool decodeJpegNoResize(unsigned char *jpegData, size_t jpegSize, void **outBuffer, size_t *outBufferSize,
                                    ImageInfo *resultInfo, JPEG_DECODE_COLORS colors);

        public:
            JpegDecoder();

            /**
             * Decodes a jpeg image to a bitmap in memory.
             * If you want a scaled version, specify the size in target_width and target_height. The image will then be scaled approximately.
             * This function can not scale images to an exact size!
             *
             * @param jpegData: pointer to jpeg data
             * @param jpegSize: size of the jpeg data buffer
             * @param outBuffer: pointer to a pointer where the target image will be stored. If the buffer is too small or null,
             *                   it will be freed and a new buffer with sufficient size will be created.
             * @param outBufferSize: pointer to the size of the out buffer size. If the out buffer is enlarged, this will be overwritten
             * @param resultInfo: info about the output image
             * @param colors: specifies the color mode of the output image
             * @param targetHeight: the height of the target image. 0 if don't care.
             * @param target_widthL the width of the target image. 0 if don't care.
             * @param scaleType: specifies if we want at least the given dimensions or maximum
             * @return true on success
             */
            bool decodeJpeg(unsigned char *jpegData, size_t jpegSize, void **outBuffer, size_t *outBufferSize,
                            ImageInfo *resultInfo, JPEG_DECODE_COLORS colors,
                            unsigned int targetWidth = 0, unsigned int targetHeight = 0,
                            JPEG_DECODE_SCALE_TYPE scaleType = SMALLER_THAN_REQUIRED);

            virtual ~JpegDecoder();
        };
    }
}

#endif //SELF_O_MAT_JPEGDECODER_H
