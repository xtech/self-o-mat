//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera) : logic(logic), camera(camera),
                                                                          server ("0.0.0.0", "9080", mux, false) {}

bool BoothApi::start() {

    mux.handle("/camera_settings")
            .get([this](served::response & res, const served::request & req) {
                CameraSettings currentCameraSettings;

                currentCameraSettings.set_aperture(camera->getAperture());
                currentCameraSettings.set_iso(camera->getIso());
                currentCameraSettings.set_shutter_speed(camera->getShutterSpeed());
                currentCameraSettings.set_exposure_compensation(camera->getExposureCorrection());
                currentCameraSettings.set_image_format(camera->getImageFormat());
                currentCameraSettings.set_lens_name(camera->getLensName());
                currentCameraSettings.set_camera_name(camera->getCameraName());

                res << currentCameraSettings.SerializeAsString();
            })
            .post([this](served::response & res, const served::request & req) {
                CameraSettings newsettings;

                if (!newsettings.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool success = true;

                success &= camera->setIso(newsettings.iso());
                success &= camera->setShutterSpeed(newsettings.shutter_speed());
                success &= camera->setAperture(newsettings.aperture());
                success &= camera->setExposureCorrection(newsettings.exposure_compensation());
                success &= camera->setImageFormat(newsettings.image_format());

                if (!success) {
                    served::response::stock_reply(400, res);
                    return;
                }

                std::cout << "Got new camera settings" << std::endl;
                newsettings.PrintDebugString();


                served::response::stock_reply(200, res);
                return;
            });
      mux.handle("/booth_settings")
            .get([this](served::response & res, const served::request & req) {
                BoothSettings currentBoothSettings;

                currentBoothSettings.set_printer_enabled(logic->getPrinterEnabled());

                res << currentBoothSettings.SerializeAsString();
            })
            .post([this](served::response & res, const served::request & req) {
                BoothSettings newsettings;

                if (!newsettings.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }


                logic->setPrinterEnabled(newsettings.printer_enabled());

                std::cout << "Got new booth settings" << std::endl;
                newsettings.PrintDebugString();

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/camera_choices")
            .get([this](served::response & res, const served::request & req) {
                CameraChoices choices;
                {
                    auto *isoChoices = camera->getIsoChoices();
                    if(isoChoices != nullptr) {
                        for (int i = 0; i < isoChoices->size(); i++) {
                            choices.add_iso_choices(isoChoices->at(i));
                        }
                    }
                }
                {
                    auto *shutterChoices = camera->getShutterSpeedChoices();
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
                    auto *cs = camera->getExposureCorrectionModeChoices();
                    if(cs != nullptr) {
                        for (int i = 0; i < cs->size(); i++) {
                            choices.add_exposure_compensation_choices(cs->at(i));
                        }
                    }
                }
                {
                    auto *cs = camera->getImageFormatChoices();
                    if(cs != nullptr) {
                        for (int i = 0; i < cs->size(); i++) {
                            choices.add_image_format_choices(cs->at(i));
                        }
                    }
                }

                res << choices.SerializeAsString();
            });

    mux.handle("/trigger")
            .post([this](served::response & res, const served::request & req) {
                logic->trigger();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/focus")
            .post([this](served::response & res, const served::request & req) {
                camera->autofocusBlocking();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/update")
            .post([this](served::response & res, const served::request & req) {
                logic->stopForUpdate();
                served::response::stock_reply(200, res);
                return;
            });


    // Create the server and run with 2 handler thread.
    server.run(2, false);

    cout << "Api started" << endl;

    return true;
}

bool BoothApi::stop() {
    cout << "Stopping the api" << endl;

    server.stop();

    return true;
}