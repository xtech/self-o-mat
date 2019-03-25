//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_NOPCAMERA_H
#define SELF_O_MAT_NOPCAMERA_H


#include "ICamera.h"
#include <opencv2/opencv.hpp>


namespace selfomat {
    namespace camera {
        class NopCamera : public ICamera {

        private:
            unsigned int preview_count = 0;
            unsigned int capture_count = 0;

        public:
            CameraStartResult start() override {
                setState(STATE_WORKING);
                return START_RESULT_SUCCESS;
            }

            bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) override {
                cv::Mat image(480, 640, CV_8UC4, cv::Scalar(30, 0, 0, 255));
                resultInfo->width = image.cols;
                resultInfo->height = image.rows;

                cv::putText(image, "# NOP CAM #", cv::Point(200, 200), CV_FONT_HERSHEY_COMPLEX, 3,
                            cv::Scalar(0, 255, 0, 255));
                cv::putText(image, std::to_string(preview_count++), cv::Point(200, 300), CV_FONT_HERSHEY_COMPLEX, 3,
                            cv::Scalar(0, 255, 0, 255));


                auto size = image.cols * image.rows * 4;
                selfomat::tools::requireBufferWithSize(buffer, bufferSize, size);

                cv::Mat outImage(image.rows, image.cols, CV_8UC4, *buffer);
                image.copyTo(outImage);

                usleep(50000);
                return true;
            }

            void stop() override {
                cout << "Stopping camera!" << endl;
                setState(STATE_STOPPED);
            }

            bool triggerCaptureBlocking() override {
                return true;
            }

            virtual bool
            readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, std::string *fullJpegFilename,
                              void **previewBuffer, size_t *previewBufferSize, ImageInfo *previewImageInfo) {
                cv::Mat image(2048, 4096, CV_8UC4, cv::Scalar(0, 255, 0, 255));

                cv::putText(image, "# CAP #", cv::Point(200, 200), CV_FONT_HERSHEY_COMPLEX, 3,
                            cv::Scalar(255, 0, 0, 255));
                cv::putText(image, std::to_string(capture_count++), cv::Point(200, 300), CV_FONT_HERSHEY_COMPLEX, 3,
                            cv::Scalar(255, 0, 0, 255));
                vector<uchar> vect;
                cv::imencode(".jpg", image, vect);

                selfomat::tools::requireBufferWithSize(fullJpegBuffer, fullJpegBufferSize, vect.size());
                memcpy(*fullJpegBuffer, vect.data(), vect.size());


                cv::Size previewSize(1024, 2048);
                auto size = previewSize.height * previewSize.width * 4;
                selfomat::tools::requireBufferWithSize(previewBuffer, previewBufferSize, size);

                cv::Mat previewImage(previewSize.height, previewSize.width, CV_8UC4, *previewBuffer);

                cv::resize(image, previewImage, previewSize);

                previewImageInfo->width = previewSize.width;
                previewImageInfo->height = previewSize.height;

                *fullJpegFilename = "nop.jpg";
                usleep(50000);
                return true;
            }

            int getIso() override {
                return 0;
            }

            int getShutterSpeed() override {
                return 0;
            }

            int getAperture() override {
                return 0;
            }

            int getShootingMode() override {
                return 0;
            }

            const vector<string> * const getIsoChoices() override {
                return nullptr;
            }

            const vector<string> * const getShutterSpeedChoices() override {
                return nullptr;
            }

            const vector<string> *const getApertureChoices() override {
                return nullptr;
            }

            const vector<string> * const getShootingModeChoices() override {
                return nullptr;
            }

            bool setIso(int iso_choice) override {
                return false;
            }

            bool setShutterSpeed(int shutter_speed_choice) override {
                return false;
            }

            bool setAperture(int aperture_choice) override {
                return false;
            }


            bool autofocusBlocking() override {
                return false;
            }

            const vector<string> * const getExposureCorrectionModeChoices() override {
                return nullptr;
            }

            const vector<string> * const getImageFormatChoices() override {
                return nullptr;
            }


            string getCameraName() override {
                return std::__cxx11::string();
            }

            string getLensName() override {
                return std::__cxx11::string();
            }

            int getExposureCorrection() override {
                return 0;
            }

            int getImageFormat() override {
                return 0;
            }

            bool setExposureCorrection(int exposure_correction_choice) override {
                return false;
            }

            bool setImageFormat(int image_format_choice) override {
                return false;
            }

            bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) override {
                return false;
            }
        };
    }
}

#endif //SELF_O_MAT_NOPCAMERA_H
