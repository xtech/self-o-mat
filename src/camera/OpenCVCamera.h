//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_OPENCVCAMERA_H
#define SELF_O_MAT_OPENCVCAMERA_H


#include "ICamera.h"
#include <opencv2/opencv.hpp>
#include "../tools/buffers.h"


using namespace cv;
using namespace std;

namespace selfomat {
    namespace camera {

        class OpenCVCamera : public ICamera {

        private:
            static std::string TAG;
            VideoCapture cap;

        public:
            CameraStartResult start() override;

            bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) override;


            void stop() override;

            bool triggerCaptureBlocking(bool autofocus_before_trigger) override;


            int getIso() override;

            int getShutterSpeed() override;

            int getAperture() override;

            int getShootingMode() override;

            CameraState getState() override;

            bool setIso(int iso_choice) override;

            bool setShutterSpeed(int shutter_speed_choice) override;

            bool setAperture(int aperture_choice) override;


            bool autofocusBlocking() override;

            string getCameraName() override;

            string getLensName() override;

            int getExposureCorrection() override;

            int getImageFormat() override;

            bool setExposureCorrection(int exposure_correction_choice) override;

            bool setImageFormat(int image_format_choice) override;

            bool readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, std::string *fullJpegFilename,
                                   void **previewBuffer,
                                   size_t *previewBufferSize, ImageInfo *previewImageInfo) override;

            bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) override;

            const vector<string> *const getApertureChoices() override;

            const vector<string> *const getIsoChoices() override;

            const vector<string> *const getShutterSpeedChoices() override;

            const vector<string> *const getShootingModeChoices() override;

            const vector<string> *const getExposureCorrectionModeChoices() override;

            const vector<string> *const getImageFormatChoices() override;

            int getExposureCorrectionTrigger() override;

            bool setExposureCorrectionTrigger(int exposure_correction_choice) override;
        };

    }
}
#endif //SELF_O_MAT_OPENCVCAMERA_H
