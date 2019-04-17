//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;
using namespace xtech::selfomat;

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera) : logic(logic), camera(camera),
                                                                          server("0.0.0.0", "9080", mux, false) {}


bool BoothApi::start() {



    mux.handle("/camera_settings/aperture")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setAperture(update.value());

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/camera_settings/iso")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setIso(update.value());

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/camera_settings/shutter_speed")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setShutterSpeed(update.value());

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/camera_settings/exposure_correction")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setExposureCorrection(update.value());

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/camera_settings/image_format")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setImageFormat(update.value());

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/camera_settings")
            .get([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                CameraSettings currentCameraSettings;
                {
                    auto setting = currentCameraSettings.mutable_aperture();
                    setting->set_update_url("/camera_settings/aperture");
                    setting->set_name("Aperture");

                    setting->set_currentindex(camera->getAperture());
                    auto *choices = camera->getApertureChoices();
                    if (choices != nullptr) {
                        for (int i = 0; i < choices->size(); i++) {
                            setting->add_values(choices->at(i));
                        }
                    }
                }

                {
                    auto setting = currentCameraSettings.mutable_iso();
                    setting->set_name("ISO");
                    setting->set_update_url("/camera_settings/iso");
                    setting->set_currentindex(camera->getIso());
                    auto *choices = camera->getIsoChoices();
                    if (choices != nullptr) {
                        for (int i = 0; i < choices->size(); i++) {
                            setting->add_values(choices->at(i));
                        }
                    }
                }

                {
                    auto setting = currentCameraSettings.mutable_shutter_speed();
                    setting->set_name("Shutter Speed");
                    setting->set_update_url("/camera_settings/shutter_speed");
                    setting->set_currentindex(camera->getShutterSpeed());
                    auto *choices = camera->getShutterSpeedChoices();
                    if (choices != nullptr) {
                        for (int i = 0; i < choices->size(); i++) {
                            setting->add_values(choices->at(i));
                        }
                    }
                }

                {
                    auto setting = currentCameraSettings.mutable_exposure_compensation();
                    setting->set_name("Exposure Compensation (during Flash)");
                    setting->set_update_url("/camera_settings/exposure_correction");
                    setting->set_currentindex(camera->getExposureCorrection());
                    auto *choices = camera->getExposureCorrectionModeChoices();
                    if (choices != nullptr) {
                        for (int i = 0; i < choices->size(); i++) {
                            setting->add_values(choices->at(i));
                        }
                    }
                }

                {
                    auto setting = currentCameraSettings.mutable_image_format();
                    setting->set_name("Image Format");
                    setting->set_update_url("/camera_settings/image_format");
                    setting->set_currentindex(camera->getImageFormat());
                    auto *choices = camera->getImageFormatChoices();
                    if (choices != nullptr) {
                        for (int i = 0; i < choices->size(); i++) {
                            setting->add_values(choices->at(i));
                        }
                    }
                }

                auto lensNameSetting = currentCameraSettings.mutable_lens_name();
                lensNameSetting->set_name("Lens Name");
                lensNameSetting->set_value(camera->getLensName());
                auto cameraNameSetting = currentCameraSettings.mutable_camera_name();
                cameraNameSetting->set_name("Camera Name");
                cameraNameSetting->set_value(camera->getCameraName());


                res << currentCameraSettings.SerializeAsString();
            });


    mux.handle("/booth_settings/printer/enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setPrinterEnabled(update.value(), true);

                cout << "updated printer enabled to: " << update.value() << endl;

                served::response::stock_reply(200, res);
                return;
            });
    mux.handle("/booth_settings/flash/enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                flashEnabled = update.value();
                logic->setFlashParameters(flashEnabled, flashBrightness, flashFade, delayMicros, durationMicros, true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/flash/brightness")
            .post([this](served::response &res, const served::request &req) {
                FloatUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                flashBrightness = update.value();
                logic->setFlashParameters(flashEnabled, flashBrightness, flashFade, delayMicros, durationMicros, true);

                served::response::stock_reply(200, res);
                return;
            });
    mux.handle("/booth_settings/flash/fade")
            .post([this](served::response &res, const served::request &req) {
                FloatUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                flashFade = update.value();
                logic->setFlashParameters(flashEnabled, flashBrightness, flashFade, delayMicros, durationMicros, true);

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/booth_settings/flash/delay")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                delayMicros = update.value();
                logic->setFlashParameters(flashEnabled, flashBrightness, flashFade, delayMicros, durationMicros, true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/flash/duration")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                durationMicros = update.value();
                logic->setFlashParameters(flashEnabled, flashBrightness, flashFade, delayMicros, durationMicros, true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/template_enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setTemplateEnabled(update.value(), true);

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/trigger")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                logic->trigger();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/focus")
            .post([this](served::response &res, const served::request &req) {
                if(camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                camera->autofocusBlocking();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/update")
            .post([this](served::response &res, const served::request &req) {
                logic->stopForUpdate();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings")
            .get([this](served::response &res, const served::request &req) {
                BoothSettings currentBoothSettings;

                {
                    auto setting = currentBoothSettings.mutable_printer_enabled();
                    setting->set_name("Printer Enabled?");
                    setting->set_update_url("/booth_settings/printer/enabled");
                    setting->set_currentvalue(logic->getPrinterEnabled());
                }

                bool flashEnabled;
                float flashBrightness, flashFade;
                uint64_t delayMicros, durationMicros;
                logic->getFlashParameters(&flashEnabled, &flashBrightness, &flashFade, &delayMicros, &durationMicros);
                {
                    auto setting = currentBoothSettings.mutable_flash_enabled();
                    setting->set_update_url("/booth_settings/flash/enabled");
                    setting->set_name("Flash Enabled?");
                    setting->set_currentvalue(flashEnabled);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_brightness();
                    setting->set_update_url("/booth_settings/flash/brightness");
                    setting->set_name("Flash Brightness");
                    setting->set_currentvalue(flashBrightness);
                    setting->set_minvalue(0.0f);
                    setting->set_maxvalue(1.0f);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_fade();
                    setting->set_update_url("/booth_settings/flash/fade");
                    setting->set_name("Flash Fade");
                    setting->set_currentvalue(flashFade);
                    setting->set_minvalue(0.0f);
                    setting->set_maxvalue(1.0f);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_delay_micros();
                    setting->set_update_url("/booth_settings/flash/delay");
                    setting->set_name("Flash Delay Microseconds");
                    setting->set_currentvalue(delayMicros);
                    setting->set_minvalue(0);
                    setting->set_maxvalue(100000);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_duration_micros();
                    setting->set_update_url("/booth_settings/flash/duration");
                    setting->set_name("Flash Duration Microseconds");
                    setting->set_currentvalue(durationMicros);
                    setting->set_minvalue(0);
                    setting->set_maxvalue(100000);
                }

                {
                    auto setting = currentBoothSettings.mutable_template_enabled();
                    setting->set_update_url("/booth_settings/template_enabled");
                    setting->set_name("Template Enabled?");
                    setting->set_currentvalue(logic->getTemplateEnabled());
                }



                res << currentBoothSettings.SerializeAsString();
            });


    mux.handle("/stress")
            .post([this](served::response &res, const served::request &req) {
                logic->enableStressTest();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/unstress")
            .post([this](served::response &res, const served::request &req) {
                logic->disableStressTest();
                served::response::stock_reply(200, res);
                return;
            });


    // Create the server and run with 2 handler thread.
    server.run(1, false);

    cout << "Api started" << endl;

    return true;
}

bool BoothApi::stop() {
    cout << "Stopping the api" << endl;

    server.stop();

    return true;
}