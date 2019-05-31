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

            const std::string cameraName = "Canon EOS 5D Mark III";
            const std::string lensName = "EF24-70mm f/4L IS USM";

            int iso = 4;
            int shutterSpeed = 0;
            int aperture = 2;
            int shootingMode = 2;
            int exposureCorrectionTrigger = 12;
            int exposureCorrection = 14;
            int imageFormat = 2;


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

                    cv::Mat flipped;
                    video >> flipped;
                    cv::flip(flipped, image, 1);
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
                usleep(30000);
                return true;
            }

            int getIso() override {
                return iso;
            }

            int getShutterSpeed() override {
                return shutterSpeed;
            }

            int getAperture() override {
                return aperture;
            }

            int getShootingMode() override {
                return shootingMode;
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
                iso = iso_choice;
                return true;
            }

            bool setShutterSpeed(int shutter_speed_choice) override {
                shutterSpeed = shutter_speed_choice;
                return true;
            }

            bool setAperture(int aperture_choice) override {
                aperture = aperture_choice;
                return true;
            }


            bool autofocusBlocking() override {
                return true;
            }

            const vector<string> * const getExposureCorrectionModeChoices() override {
                return &exposureCorrectionModeChoices;
            }

            const vector<string> * const getImageFormatChoices() override {
                return &imageFormatChoices;
            }


            string getCameraName() override {
                return cameraName;
            }

            string getLensName() override {
                return lensName;
            }

            int getExposureCorrection() override {
                return exposureCorrection;
            }

            int getImageFormat() override {
                return imageFormat;
            }

            bool setExposureCorrection(int exposure_correction_choice) override {
                exposureCorrection = exposure_correction_choice;
                return true;
            }

            bool setImageFormat(int image_format_choice) override {
                imageFormat = image_format_choice;
                return true;
            }

            bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) override {
                return false;
            }

            int getExposureCorrectionTrigger() override {
                return exposureCorrectionTrigger;
            }

            bool setExposureCorrectionTrigger(int exposure_correction_choice) override {
                exposureCorrectionTrigger = exposure_correction_choice;
                return true;
            }
        };
    }
}

#endif //SELF_O_MAT_NOPCAMERA_H
