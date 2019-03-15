//
// Created by clemens on 21.01.19.
//

#ifndef SELF_O_MAT_ICAMERA_H
#define SELF_O_MAT_ICAMERA_H


#include <ui/BoothGui.h>
#include <tools/imageinfo.h>
#include <iostream>

using namespace std;

enum CameraStartResult {
    START_RESULT_SUCCESS,
    START_RESULT_ERROR,
    START_RESULT_NOT_FOUND
};

enum CameraState {
    STATE_STOPPED,
    STATE_WORKING,
    STATE_ERROR
};

enum CameraShootingMode {
    MODE_AUTO,
    MODE_MANUAL,
    MODE_OTHER
};

class ICamera {

private:
    boost::mutex stateMutex;
    CameraState currentState{};


public:

    /**
     * Starts the camera.
     * @return a CameraStartResult
     */
    virtual CameraStartResult start() = 0;

    virtual void stop() = 0;
    /**
     * Returns the current state of the camera
     * @return
     */
    CameraState getState() {
        stateMutex.lock();
        auto stateCopy = currentState;
        stateMutex.unlock();
        return stateCopy;
    }

    /**
     * Captures a preview image into the specified buffer.
     * If the buffer is large enough, we simply write the data into it
     * else, the buffer will be deleted and replaced by a new, larger buffer.
     *
     * So expect the pointer to change!
     * @param buffer: The buffer to use
     * @param bufferSize: The size of the current buffer
     * @param resultInfo: A pointer to the resulting image info
     * @return true if success else false
     */
    virtual bool capturePreviewBlocking(void **buffer, size_t *bufferSize, ImageInfo *resultInfo) = 0;

    /**
     * Captures an image into the specified buffer.
     * If the buffer is large enough, we simply write the data into it
     * else, the buffer will be deleted and replaced by a new, larger buffer.
     *
     * So expect the pointer to change!
     * @param buffer: The buffer to use
     * @param bufferSize: The size of the current buffer
     * @param resultInfo: A pointer to the resulting image info
     * @return true if success else false
     */
    virtual bool triggerCaptureBlocking() = 0;

    /**
     * Captures an image into the specified buffer.
     * If the buffer is large enough, we simply write the data into it
     * else, the buffer will be deleted and replaced by a new, larger buffer.
     *
     * So expect the pointer to change!
     * @param buffer: The buffer to use
     * @param bufferSize: The size of the current buffer
     * @param resultInfo: A pointer to the resulting image info
     * @return true if success else false
     */
    virtual bool readImageBlocking(void **fullJpegBuffer, size_t *fullJpegBufferSize, std::string *fullJpegFilename, void **previewBuffer, size_t *previewBufferSize, ImageInfo *previewImageInfo) = 0;

    void setState(CameraState state) {
        stateMutex.lock();
        currentState = state;
        stateMutex.unlock();
    }

    virtual int getIso() = 0;
    virtual int getShutterSpeed() = 0;
    virtual int getAperture() = 0;
    virtual int getShootingMode() = 0;
    virtual int getExposureCorrection() = 0;
    virtual int getImageFormat() = 0;
    virtual int getImageFormatSd() = 0;

    virtual vector<string> *getIsoChoices() = 0;
    virtual vector<string> *getShutterSpeedChoices() = 0;
    virtual vector<string> *getApertureChoices() = 0;
    virtual vector<string> *getShootingModeChoices() = 0;
    virtual vector<string> *getExposureCorrectionModeChoices()  = 0;
    virtual vector<string> *getImageFormatChoices()  = 0;
    virtual vector<string> *getImageFormatSdChoices()  = 0;
    virtual string getCameraName() = 0;
    virtual string getLensName() = 0;

    virtual bool setIso(int iso_choice) = 0;
    virtual bool setShutterSpeed(int shutter_speed_choice) = 0;
    virtual bool setAperture(int aperture_choice) = 0;
    virtual bool setExposureCorrection(int exposure_correction_choice) = 0;
    virtual bool setImageFormat(int image_format_choice) = 0;
    virtual bool setImageFormatSd(int image_format_sd_choice) = 0;

    virtual bool autofocusBlocking() = 0;

    virtual bool getLastRawImage(void **targetBuffer, size_t *targetSize, std::string *filename) = 0;

    virtual ~ICamera() = default;
};


#endif //SELF_O_MAT_ICAMERA_H
