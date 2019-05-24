//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;
using namespace xtech::selfomat;

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera, bool show_led_setup) : logic(logic), camera(camera),
                                                                          server("0.0.0.0", "9080", mux, false)
{
    this->show_led_setup = show_led_setup;
}

void BoothApi::setHeaders(served::response &res) {
    for (auto const &h : this->headers)
        res.set_header(h.first, h.second);
}

bool BoothApi::start() {

    mux.handle("/camera_settings/aperture")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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

    mux.handle("/camera_settings/exposure_correction_trigger")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                camera->setExposureCorrectionTrigger(update.value());

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/camera_settings/image_format")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
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
                    setting->set_name("Live Brightness");
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
                    auto setting = currentCameraSettings.mutable_exposure_compensation_trigger();
                    setting->set_name("Capture Brightness");
                    setting->set_update_url("/camera_settings/exposure_correction_trigger");
                    setting->set_currentindex(camera->getExposureCorrectionTrigger());
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

                {
                    auto setting = currentCameraSettings.mutable_focus();
                    setting->set_name("Autofocus");
                    setting->set_post_url("/focus");
                }

                auto lensNameSetting = currentCameraSettings.mutable_lens_name();
                lensNameSetting->set_name("Lens Name");
                lensNameSetting->set_value(camera->getLensName());
                auto cameraNameSetting = currentCameraSettings.mutable_camera_name();
                cameraNameSetting->set_name("Camera Name");
                cameraNameSetting->set_value(camera->getCameraName());


                res << currentCameraSettings.SerializeAsString();
            });

    mux.handle("/booth_settings/storage/enabled")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setStorageEnabled(update.value(), true);

                cout << "updated storage enabled to: " << update.value() << endl;

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/printer/enabled")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

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
                this->setHeaders(res);

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
                this->setHeaders(res);

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
                this->setHeaders(res);

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
                this->setHeaders(res);

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
                this->setHeaders(res);

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
                this->setHeaders(res);

                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setTemplateEnabled(update.value(), true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/led_offset")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                int ledCount = logic->getLEDCount() / 2;
                logic->setLEDOffset(update.value()-ledCount, true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/countdown_duration")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setCountdownDuration(update.value()+1, true);

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings/led_mode")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                int i=0;
                for ( const auto e : selfomat::logic::LED_MODE_ALL ) {
                    if (i == update.value()) {
                        logic->setLEDMode(e.first, true);
                    }
                    i++;
                }

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/booth_settings/led_count")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                int i=0;
                for ( const auto e : selfomat::logic::LED_COUNT_ALL ) {
                    if (i == update.value()) {
                        logic->setLEDCount(e.first, true);
                    }
                    i++;
                }

                served::response::stock_reply(200, res);
                return;
            });


    mux.handle("/trigger")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                logic->trigger();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/cancel_print")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                logic->cancelPrint();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/focus")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                if (camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                logic->adjustFocus();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/flash")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                logic->flashTest();

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/update")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                logic->stopForUpdate();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/template_upload")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                cout << "New template with size: " << req.body().size() << endl;

                // TODO: Save, show and use new template

                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/booth_settings")
            .get([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                BoothSettings currentBoothSettings;

                {
                    auto setting = currentBoothSettings.mutable_storage_enabled();
                    setting->set_name("USB Storage Enabled?");
                    setting->set_update_url("/booth_settings/storage/enabled");
                    setting->set_currentvalue(logic->getStorageEnabled());
                }

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

//                {
//                    auto setting = currentBoothSettings.mutable_flash_brightness();
//                    setting->set_update_url("/booth_settings/flash/brightness");
//                    setting->set_name("Flash Brightness");
//                    setting->set_currentvalue(flashBrightness);
//                    setting->set_minvalue(0.0f);
//                    setting->set_maxvalue(1.0f);
//                }
//
//                {
//                    auto setting = currentBoothSettings.mutable_flash_fade();
//                    setting->set_update_url("/booth_settings/flash/fade");
//                    setting->set_name("Flash Fade");
//                    setting->set_currentvalue(flashFade);
//                    setting->set_minvalue(0.0f);
//                    setting->set_maxvalue(1.0f);
//                }
//
//                {
//                    auto setting = currentBoothSettings.mutable_flash_delay_micros();
//                    setting->set_update_url("/booth_settings/flash/delay");
//                    setting->set_name("Flash Delay Microseconds");
//                    setting->set_currentvalue(delayMicros);
//                    setting->set_minvalue(0);
//                    setting->set_maxvalue(100000);
//                }

                {
                    auto setting = currentBoothSettings.mutable_flash_duration_micros();
                    setting->set_update_url("/booth_settings/flash/duration");
                    setting->set_name("Flash Brightness");
                    setting->set_currentvalue(durationMicros);
                    setting->set_minvalue(0);
                    setting->set_maxvalue(255);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_test();
                    setting->set_name("Flash Test");
                    setting->set_post_url("/flash");
                }

                {
                        auto setting = currentBoothSettings.mutable_template_upload();
                        setting->set_post_url("/template_upload");
                        setting->set_name("Template Upload");
                        setting->set_input_accept("image/x-png,image/png");
                }


                if(logic->getTemplateLoaded()) {
                    {
                        auto setting = currentBoothSettings.mutable_template_enabled();
                        setting->set_update_url("/booth_settings/template_enabled");
                        setting->set_name("Template Enabled?");
                        setting->set_currentvalue(logic->getTemplateEnabled());
                    }
                }

                if (this->show_led_setup) {
                    {
                        auto setting = currentBoothSettings.mutable_led_mode();
                        setting->set_update_url("/booth_settings/led_mode");
                        setting->set_name("LED Mode");
                        setting->set_currentindex(0);

                        int i=0;
                        for ( const auto e : selfomat::logic::LED_MODE_ALL ) {
                            if (e.first == logic->getLEDMode()) {
                                setting->set_currentindex(i);
                            }
                            setting->add_values(e.second);
                            i++;
                        }
                    }

                    {
                        auto setting = currentBoothSettings.mutable_led_count();
                        setting->set_update_url("/booth_settings/led_count");
                        setting->set_name("LED Count");
                        setting->set_currentindex(0);

                        int i=0;
                        for ( const auto e : selfomat::logic::LED_COUNT_ALL ) {
                            if (e.first == logic->getLEDCount()) {
                                setting->set_currentindex(i);
                            }
                            setting->add_values(e.second);
                            i++;
                        }
                    }
                }


                {
                    auto setting = currentBoothSettings.mutable_led_offset();
                    setting->set_update_url("/booth_settings/led_offset");
                    setting->set_name("LED Offset");

                    int ledCount = logic->getLEDCount() / 2;
                    setting->set_currentindex(ledCount);

                    for (int i = ledCount * -1; i <= ledCount; i++) {
                        if (i == logic->getLEDOffset()) {
                            setting->set_currentindex(i+ledCount);
                        }
                        setting->add_values(std::to_string(i));
                    }
                }

                {
                    auto setting = currentBoothSettings.mutable_countdown_duration();
                    setting->set_update_url("/booth_settings/countdown_duration");
                    setting->set_name("Countdown Duration");

                    setting->set_currentindex(logic->getCountdownDuration()-1);

                    for (int i = 1; i <= 5; i++) {
                        setting->add_values(std::to_string(i)+"s");
                    }
                }

                {
                    auto setting = currentBoothSettings.mutable_update_mode();
                    setting->set_name("Update Mode");
                    setting->set_post_url("/update");
                    setting->set_alert("Do you really want to start the Update Mode?");
                }


                res << currentBoothSettings.SerializeAsString();
            });


    mux.handle("/stress")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                logic->enableStressTest();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/unstress")
            .post([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                logic->disableStressTest();
                served::response::stock_reply(200, res);
                return;
            });

    mux.handle("/version")
            .get([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                std::string filename = "./version";

                ifstream f(filename, ios::in);
                string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                res.set_status(200);
                res.set_body(file_contents);
            });


    mux.handle("/app/svg/{file}")
            .get([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                std::string filename = "./app/svg/" + req.params["file"];

                res.set_header("Content-Type", "image/svg+xml");

                ifstream f(filename, ios::in);
                string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                res.set_status(200);
                res.set_body(file_contents);
            });

    mux.handle("/app/{file}")
            .get([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                string file = req.params["file"];

                if (file.compare("tabs") == 0) {
                    res.set_status(301);
                    res.set_header("Location", "/app/index.html");
                } else {
                    ifstream f("./app/" + file, ios::in);
                    string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                    res.set_status(200);
                    res.set_body(file_contents);
                }
            });

    mux.handle("/{file}")
            .get([this](served::response &res, const served::request &req) {
                this->setHeaders(res);

                string file = req.params["file"];

                if (file.compare("app") == 0) {
                    res.set_status(301);
                    res.set_header("Location", "/app/index.html");
                }
            });

    // Create the server and run with 2 handler thread.
    // THIS IS NEEDED BECAUSE BLOCKING=FALSE IS IGNORED BY SERVERD IF THREADS = 1!!!!
    server.run(2, false);

    cout << "Api started" << endl;

    return true;
}

bool BoothApi::stop() {
    cout << "Stopping the api" << endl;

    server.stop();

    return true;
}