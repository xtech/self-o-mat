//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;

bool BoothApi::start() {


    httpEndpoint.init();

    Rest::Router router;

    Rest::Routes::Get(router, "/camera_settings", Rest::Routes::bind(&BoothApi::getCurrentCameraSettings, this));
    Rest::Routes::Get(router, "/camera_choices", Rest::Routes::bind(&BoothApi::getCameraChoices, this));
    Rest::Routes::Post(router, "/camera_settings", Rest::Routes::bind(&BoothApi::setCameraSettings, this));
    Rest::Routes::Post(router, "/trigger", Rest::Routes::bind(&BoothApi::triggerCapture, this));
    Rest::Routes::Post(router, "/focus", Rest::Routes::bind(&BoothApi::autofocus, this));


    httpEndpoint.setHandler(router.handler());
    httpEndpoint.serveThreaded();

    return true;
}


void BoothApi::getCurrentCameraSettings(const Rest::Request &request, Http::ResponseWriter response) {
    CameraSettings currentCameraSettings;

    currentCameraSettings.set_aperture(camera->getAperture());
    currentCameraSettings.set_iso(camera->getIso());
    currentCameraSettings.set_shutter_speed(camera->getShutterSpeed());
    currentCameraSettings.set_exposure_compensation(camera->getExposureCorrection());
    currentCameraSettings.set_image_format(camera->getImageFormat());
    currentCameraSettings.set_image_format_sd(camera->getImageFormatSd());
    currentCameraSettings.set_lens_name(camera->getLensName());
    currentCameraSettings.set_camera_name(camera->getCameraName());

    response.send(Http::Code::Ok, currentCameraSettings.SerializeAsString());
}

void BoothApi::setCameraSettings(const Rest::Request &request, Http::ResponseWriter response) {
    CameraSettings newsettings;

    if (!newsettings.ParseFromString(request.body())) {
        response.send(Http::Code::Bad_Request);
        return;
    }

    bool success = true;

    success &= camera->setIso(newsettings.iso());
    success &= camera->setShutterSpeed(newsettings.shutter_speed());
    success &= camera->setAperture(newsettings.aperture());
    success &= camera->setExposureCorrection(newsettings.exposure_compensation());
    success &= camera->setImageFormat(newsettings.image_format());
    success &= camera->setImageFormatSd(newsettings.image_format_sd());


    if (success) {
        response.send(Http::Code::Bad_Request);
        return;
    }

    std::cout << "Got new camera settings" << std::endl;
    newsettings.PrintDebugString();

    response.send(Http::Code::Ok);
}

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera) : logic(logic), camera(camera),
                                                         httpEndpoint(Address(Ipv4::any(), 9080)) {}

void BoothApi::triggerCapture(const Rest::Request &request, Http::ResponseWriter response) {
    logic->trigger();
    response.send(Http::Code::Ok);
}

void BoothApi::autofocus(const Rest::Request &request, Http::ResponseWriter response) {
    camera->autofocusBlocking();
    response.send(Http::Code::Ok);
}



void BoothApi::getCameraChoices(const Rest::Request &request, Http::ResponseWriter response) {
    CameraChoices choices;
    {
        vector<string> *isoChoices = camera->getIsoChoices();
        if(isoChoices != nullptr) {
            for (int i = 0; i < isoChoices->size(); i++) {
                choices.add_iso_choices(isoChoices->at(i));
            }
        }
    }
    {
        vector<string> *shutterChoices = camera->getShutterSpeedChoices();
        if(shutterChoices != nullptr) {
            for (int i = 0; i < shutterChoices->size(); i++) {
                choices.add_shutter_speed_choices(shutterChoices->at(i));
            }
        }
    }
    {
        auto *apertureChoices = camera->getApertureChoices();
        if(apertureChoices != nullptr) {
            for (int i = 0; i < apertureChoices->size(); i++) {
                choices.add_aperture_choices(apertureChoices->at(i));
            }
        }
    }
    {
        vector<string> *cs = camera->getExposureCorrectionModeChoices();
        if(cs != nullptr) {
            for (int i = 0; i < cs->size(); i++) {
                choices.add_exposure_compensation_choices(cs->at(i));
            }
        }
    }
    {
        vector<string> *cs = camera->getImageFormatChoices();
        if(cs != nullptr) {
            for (int i = 0; i < cs->size(); i++) {
                choices.add_image_format_choices(cs->at(i));
            }
        }
    }
    {
        vector<string> *cs = camera->getImageFormatSdChoices();
        if(cs != nullptr) {
            for (int i = 0; i < cs->size(); i++) {
                choices.add_image_format_sd_choices(cs->at(i));
            }
        }
    }
    response.send(Http::Code::Ok, choices.SerializeAsString());
}

bool BoothApi::stop() {
    cout << "Stopping the api" << endl;
    httpEndpoint.shutdown();
    return true;
}
