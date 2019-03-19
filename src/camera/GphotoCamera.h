//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_GPHOTOCAMERA_H
#define SELF_O_MAT_GPHOTOCAMERA_H

#include <iostream>
#include <camera/ICamera.h>
#include <opencv2/opencv.hpp>
#include <sys/stat.h>
#include <boost/algorithm/string/predicate.hpp>
#include <tools/buffers.h>
#include <tools/JpegDecoder.h>
#include <camera/controllers/FocusController.h>
#include <camera/controllers/InfoController.h>
#include <camera/controllers/TriggerController.h>
#include <mutex>

extern "C" {
#include <gphoto2/gphoto2.h>
#include <gphoto2-version.h>
}

using namespace std;

namespace selfomat {
    namespace camera {
        namespace gphoto {
            class GphotoCamera : public ICamera {
            private:
                static const std::string TAG;

                TriggerController *triggerController = nullptr;
                FocusController *focusController = nullptr;
                InfoController *cameraInfoController = nullptr;

                JpegDecoder jpegDecoder;

                GPContext *gp = NULL;
                Camera *camera = NULL;

                boost::thread cameraEventThreadHandle;
                boost::posix_time::ptime lastDrainTime;
                boost::posix_time::ptime focusStartedTime;

                void *latestBuffer = nullptr;
                size_t latestBufferSize = 0;
                ImageInfo latestResultInfo{};
                std::string latestFileName;

                void *tmpBuffer = nullptr;
                size_t tmpBufferSize = 0;

                // Buffer for the last raw image received.
                boost::mutex rawBufferMutex;
                void *latestRawBuffer = nullptr;
                size_t latestRawBufferSize = 0;
                std::string latestRawFileName;

                boost::mutex cameraIoMutex;

                CameraWidget *rootWidget;


                bool settings_dirty = false;
                bool trigger_focus = false;
                bool focus_active = false;

                void drainEventQueue(bool waitForPhoto);

                void drainEventQueueWhenNeeded();

                void pullCameraSettings();

                bool pushCameraSettings();

                bool createCameraControllers();

                bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) override;

            public:
                CameraStartResult start() override;

                bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) override;

                void stop() override;

                bool triggerCaptureBlocking() override;

                bool autofocusBlocking() override;

                bool readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, std::string *fullJpegFilename,
                                       void **previewBuffer,
                                       size_t *previewBufferSize, ImageInfo *previewImageInfo) override;

                int getIso() override;

                int getShutterSpeed() override;

                int getAperture() override;

                int getShootingMode() override;

                bool setIso(int iso_choice) override;

                bool setShutterSpeed(int shutter_speed_choice) override;

                bool setAperture(int aperture_choice) override;


                vector<string> *getIsoChoices() override;

                vector<string> *getShutterSpeedChoices() override;

                vector<string> *getApertureChoices() override;

                vector<string> *getShootingModeChoices() override;


                vector<string> *getExposureCorrectionModeChoices() override;

                vector<string> *getImageFormatChoices() override;

                vector<string> *getImageFormatSdChoices() override;

                string getCameraName() override;

                string getLensName() override;

                int getExposureCorrection() override;

                int getImageFormat() override;

                int getImageFormatSd() override;

                bool setExposureCorrection(int exposure_correction_choice) override;

                bool setImageFormat(int image_format_choice) override;

                bool setImageFormatSd(int image_format_sd_choice) override;

                ~GphotoCamera() override;
            };
        }
    }
}

#endif //SELF_O_MAT_GPHOTOCAMERA_H
