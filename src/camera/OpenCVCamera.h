//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_OPENCVCAMERA_H
#define SELF_O_MAT_OPENCVCAMERA_H


#include <cv.hpp>
#include "ICamera.h"
#include <opencv2/opencv.hpp>
#include "../tools/buffers.h"


using namespace cv;
using namespace std;

class OpenCVCamera : public ICamera {

private:
    VideoCapture cap;

public:
    CameraStartResult start() override;

    bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) override;


    void stop() override;

    bool triggerCaptureBlocking() override;



    int getIso() override;

    int getShutterSpeed() override;

    int getAperture() override;

    int getShootingMode() override;

    vector<string> *getIsoChoices() override;

    vector<string> *getShutterSpeedChoices() override;

    vector<string> *getApertureChoices() override;

    vector<string> *getShootingModeChoices() override;

    bool setIso(int iso_choice) override;

    bool setShutterSpeed(int shutter_speed_choice) override;

    bool setAperture(int aperture_choice) override;



    bool autofocusBlocking() override;

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

    bool readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, void **previewBuffer,
                           size_t *previewBufferSize, ImageInfo *previewImageInfo) override;

    bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) override;
};


#endif //SELF_O_MAT_OPENCVCAMERA_H
