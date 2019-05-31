//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_NOPCAMERA_H
#define SELF_O_MAT_NOPCAMERA_H


#include "ICamera.h"
#include <opencv2/opencv.hpp>
#include <SFML/System/Clock.hpp>


namespace selfomat {
    namespace camera {
        class NopCamera : public ICamera {

        private:
            sf::Clock fpsClock;
            cv::VideoCapture video;
            boost::mutex cameraMutex;

        public:
            CameraStartResult start() override {
                setState(STATE_WORKING);

                // Load assets
                video = cv::VideoCapture("/opt/assets/nop.mov");
                if (!video.isOpened()) {
                    cerr << "Could not load nop video." << endl;
                } else {
                    fpsClock.restart();
                }

                return START_RESULT_SUCCESS;
            }

            bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) override {

                cameraMutex.lock();

                cv::Mat image;
                if (video.isOpened()) {
                    if (video.get(CV_CAP_PROP_POS_FRAMES) == video.get(CV_CAP_PROP_FRAME_COUNT)) {
                        video.set(CV_CAP_PROP_POS_FRAMES, 0);
                    }

                    video >> image;
                    cv::resize(image, image, cv::Size(1280,800));
                } else {
                    image = cv::Mat(480, 640, CV_8UC4, cv::Scalar(30, 0, 0, 255));
                }

                resultInfo->width = image.cols;
                resultInfo->height = image.rows;

                auto size = image.cols * image.rows * 4;
                selfomat::tools::requireBufferWithSize(buffer, bufferSize, size);

                cv::Mat outImage(image.rows, image.cols, CV_8UC4, *buffer);
                if (video.isOpened()) {
                    cv::cvtColor(image, outImage, CV_RGB2BGRA);

                    sf::Int32 elapsedTime = fpsClock.getElapsedTime().asMicroseconds();
                    fpsClock.restart();
                    auto timeToWait = (1000000 / video.get(CV_CAP_PROP_FPS)) - elapsedTime;
                    cameraMutex.unlock();
                    if (timeToWait > 0) {
                        usleep(timeToWait);
                    }
                } else {
                    image.copyTo(outImage);
                    cameraMutex.unlock();
                    usleep(50000);
                }
                
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

                cameraMutex.lock();

                cv::Mat image;
                if (video.isOpened()) {
                    video.retrieve(image);
                } else {
                    image = cv::Mat(2048, 4096, CV_8UC4, cv::Scalar(0, 255, 0, 255));
                }

                vector<uchar> vect;
                cv::imencode(".jpg", image, vect);

                selfomat::tools::requireBufferWithSize(fullJpegBuffer, fullJpegBufferSize, vect.size());
                memcpy(*fullJpegBuffer, vect.data(), vect.size());

                cv::Size previewSize;
                if (video.isOpened()) {
                    previewSize = cv::Size(image.cols, image.rows);
                } else {
                    previewSize = cv::Size(1024, 2048);
                }

                auto size = previewSize.height * previewSize.width * 4;
                selfomat::tools::requireBufferWithSize(previewBuffer, previewBufferSize, size);

                cv::Mat previewImage(previewSize.height, previewSize.width, CV_8UC4, *previewBuffer);

                if (video.isOpened()) {
                    cv::cvtColor(image, previewImage, CV_RGB2BGRA);
                } else {
                    cv::resize(image, previewImage, previewSize);
                }

                previewImageInfo->width = previewSize.width;
                previewImageInfo->height = previewSize.height;

                *fullJpegFilename = "nop.jpg";
                cameraMutex.unlock();
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

            int getExposureCorrectionTrigger() override {
                return 0;
            }

            bool setExposureCorrectionTrigger(int exposure_correction_choice) override {
                return false;
            }
        };
    }
}

#endif //SELF_O_MAT_NOPCAMERA_H
