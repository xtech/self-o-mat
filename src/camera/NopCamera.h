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
            const vector<string> isoChoices { "Auto", "100", "125", "160", "200", "250", "320", "400", "500", "640", "800", "1000" };
            const vector<string> shutterSpeedChoices { "Auto" };
            const vector<string> apertureChoices { "1.8", "2", "2.2", "2.5", "2.8", "3.2", "3.5", "4", "4.5", "5", "5.6", "6.3", "7.1", "8", "9", "10", "11", "13", "14", "16", "18", "20", "22" };
            const vector<string> shootingModeChoices { "P", "TV", "AV", "Manual", "Bulb" };
            const vector<string> exposureCorrectionModeChoices { "-5", "-4.6", "-4.3", "-4", "-3.6", "-3.3", "-3", "-2.6", "-2.3", "-2", "-1.6", "-1.3", "-1", "-0.6", "-0.3", "0", "0.3", "0.6", "1", "1.3", "1.6", "2", "2.3", "2.6", "3", "3.3", "3.6", "4", "4.3", "4.6", "5" };
            const vector<string> imageFormatChoices { "RAW + Tiny JPEG", "RAW + Medium Fine JPEG", "RAW + Large Fine JPEG" };

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
                return 4;
            }

            int getShutterSpeed() override {
                return 0;
            }

            int getAperture() override {
                return 2;
            }

            int getShootingMode() override {
                return 2;
            }

            const vector<string> * const getIsoChoices() override {
                return &isoChoices;
            }

            const vector<string> * const getShutterSpeedChoices() override {
                return &shutterSpeedChoices;
            }

            const vector<string> *const getApertureChoices() override {
                return &apertureChoices;
            }

            const vector<string> * const getShootingModeChoices() override {
                return &shootingModeChoices;
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
                return &exposureCorrectionModeChoices;
            }

            const vector<string> * const getImageFormatChoices() override {
                return &imageFormatChoices;
            }


            string getCameraName() override {
                return "Canon EOS 5D Mark III";
            }

            string getLensName() override {
                return "EF24-70mm f/4L IS USM";
            }

            int getExposureCorrection() override {
                return 14;
            }

            int getImageFormat() override {
                return 2;
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
                return 12;
            }

            bool setExposureCorrectionTrigger(int exposure_correction_choice) override {
                return false;
            }
        };
    }
}

#endif //SELF_O_MAT_NOPCAMERA_H
