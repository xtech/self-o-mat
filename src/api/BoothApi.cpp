//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;
using namespace xtech::selfomat;
using namespace selfomat::logic;

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera, bool show_led_setup) : logic(logic),
                                                                                               camera(camera),
                                                                                               server("0.0.0.0", "9080",
                                                                                                      mux, false) {
    this->show_led_setup = show_led_setup;
}

bool BoothApi::start() {

    // Use wrapper to set needed headers
    mux.use_wrapper([this](served::response &res, const served::request &req, std::function<void()> old) {
        for (auto const &h : this->headers)
            res.set_header(h.first, h.second);

        if (req.method() == served::OPTIONS) {
            served::response::stock_reply(200, res);
        } else {
            old();
        }
    });


    mux.handle("/camera_settings/aperture")
            .post([this](served::response &res, const served::request &req) {
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
            });

    mux.handle("/camera_settings/iso")
            .post([this](served::response &res, const served::request &req) {
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
            });

    mux.handle("/camera_settings/shutter_speed")
            .post([this](served::response &res, const served::request &req) {
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
            });

    mux.handle("/camera_settings/exposure_correction")
            .post([this](served::response &res, const served::request &req) {
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
            });

    mux.handle("/camera_settings/exposure_correction_trigger")
            .post([this](served::response &res, const served::request &req) {
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
            });


    mux.handle("/camera_settings/image_format")
            .post([this](served::response &res, const served::request &req) {
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
            });

    mux.handle("/camera_settings")
            .get([this](served::response &res, const served::request &req) {
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
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setStorageEnabled(update.value(), true);

                cout << "updated storage enabled to: " << update.value() << endl;

                served::response::stock_reply(200, res);
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
            });

    mux.handle("/booth_settings/flash/enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setFlashEnabled(update.value(), true);

                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/flash/duration")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                SelfomatController *controller = logic->getSelfomatController();
                controller->setFlashDurationMicros(update.value());
                controller->commit();

                served::response::stock_reply(200, res);
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
            });


    mux.handle("/booth_settings/led_offset_cw")
            .post([this](served::response &res, const served::request &req) {

                SelfomatController *controller = logic->getSelfomatController();
                controller->moveOffsetRight();
                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/led_offset_ccw")
            .post([this](served::response &res, const served::request &req) {

                SelfomatController *controller = logic->getSelfomatController();
                controller->moveOffsetLeft();
                served::response::stock_reply(200, res);
            });



    mux.handle("/booth_settings/countdown_duration")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                SelfomatController *controller = logic->getSelfomatController();
                controller->setCountDownMillis((update.value() + 1) * 1000);
                controller->commit();

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings/led_mode")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }


                SelfomatController *controller = logic->getSelfomatController();
                switch (update.value()) {
                    case 0:
                        controller->setLedType(SelfomatController::LED_TYPE::RGB.controllerValue);
                        break;
                    case 1:
                        controller->setLedType(SelfomatController::LED_TYPE::RGBW.controllerValue);
                        break;
                    default:
                        served::response::stock_reply(400, res);
                        return;
                }
                controller->commit();

                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/led_count")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                SelfomatController *controller = logic->getSelfomatController();
                switch (update.value()) {
                    case 0:
                        controller->setLedCount(SelfomatController::LED_COUNT::COUNT_12.controllerValue);
                        break;
                    case 1:
                        controller->setLedCount(SelfomatController::LED_COUNT::COUNT_16.controllerValue);
                        break;
                    case 2:
                        controller->setLedCount(SelfomatController::LED_COUNT::COUNT_24.controllerValue);
                        break;
                    case 3:
                        controller->setLedCount(SelfomatController::LED_COUNT::COUNT_32.controllerValue);
                        break;
                    default:
                        served::response::stock_reply(400, res);
                        return;
                }
                controller->commit();

                served::response::stock_reply(200, res);
            });


    mux.handle("/trigger")
            .post([this](served::response &res, const served::request &req) {
                if (camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                auto controller = logic->getSelfomatController();
                controller->remoteTrigger();
                served::response::stock_reply(200, res);
            });

    mux.handle("/cancel_print")
            .post([this](served::response &res, const served::request &req) {
                logic->cancelPrint();
                served::response::stock_reply(200, res);
            });

    mux.handle("/focus")
            .post([this](served::response &res, const served::request &req) {
                if (camera->getState() != STATE_WORKING) {
                    served::response::stock_reply(503, res);
                    return;
                }

                logic->adjustFocus();
                served::response::stock_reply(200, res);
            });

    mux.handle("/flash")
            .post([this](served::response &res, const served::request &req) {

                SelfomatController *controller = logic->getSelfomatController();
                controller->triggerFlash();

                served::response::stock_reply(200, res);
            });

    mux.handle("/update")
            .post([this](served::response &res, const served::request &req) {
                logic->stopForUpdate();
                served::response::stock_reply(200, res);
            });

    mux.handle("/template_upload")
            .post([this](served::response &res, const served::request &req) {

                string body =  req.body();

                if (!logic->updateTemplate((void *)body.c_str(), body.size())) {

                    BoothError error;
                    error.set_title("Template");
                    error.set_message("Keine Transparenz gefunden! Nur an transparenten Stellen im Template wird das Foto sichtbar.");
                    error.set_code(400);

                    res << error.SerializeAsString();

                    return;
                }

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings")
            .get([this](served::response &res, const served::request &req) {
                BoothSettings currentBoothSettings;

                auto controller = logic->getSelfomatController();
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

                bool flashEnabled = logic->getFlashEnabled();
                {
                    auto setting = currentBoothSettings.mutable_flash_enabled();
                    setting->set_update_url("/booth_settings/flash/enabled");
                    setting->set_name("Flash Enabled?");
                    setting->set_currentvalue(flashEnabled);
                }

                {
                    auto setting = currentBoothSettings.mutable_flash_duration_micros();
                    setting->set_update_url("/booth_settings/flash/duration");
                    setting->set_name("Flash Brightness");
                    setting->set_currentvalue(controller->getFlashDurationMicros());
                    setting->set_minvalue(0);
                    setting->set_maxvalue(100000);
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


                if (logic->getTemplateLoaded()) {
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
                        setting->set_currentindex(controller->getLedType());

                        setting->add_values(SelfomatController::LED_TYPE::RGB.humanName);
                        setting->add_values(SelfomatController::LED_TYPE::RGBW.humanName);
                    }

                    {
                        auto setting = currentBoothSettings.mutable_led_count();
                        setting->set_update_url("/booth_settings/led_count");
                        setting->set_name("LED Count");


                        switch (controller->getLedCount()) {
                            case 12:
                                setting->set_currentindex(0);
                                break;
                            case 16:
                                setting->set_currentindex(1);
                                break;
                            case 24:
                                setting->set_currentindex(2);
                                break;
                            case 32:
                                setting->set_currentindex(3);
                                break;
                            default:
                                setting->set_currentindex(0);
                                break;
                        }

                        setting->add_values(SelfomatController::LED_COUNT::COUNT_12.humanName);
                        setting->add_values(SelfomatController::LED_COUNT::COUNT_16.humanName);
                        setting->add_values(SelfomatController::LED_COUNT::COUNT_24.humanName);
                        setting->add_values(SelfomatController::LED_COUNT::COUNT_32.humanName);
                    }
                }


                {
                    auto setting = currentBoothSettings.mutable_led_offset_clockwise();
                    setting->set_post_url("/booth_settings/led_offset_cw");
                    setting->set_name("LED Offset +");
                }

                {
                    auto setting = currentBoothSettings.mutable_led_offset_counter_clockwise();
                    setting->set_post_url("/booth_settings/led_offset_ccw");
                    setting->set_name("LED Offset -");
                }


                {
                    auto setting = currentBoothSettings.mutable_countdown_duration();
                    setting->set_update_url("/booth_settings/countdown_duration");
                    setting->set_name("Countdown Duration");

                    setting->set_currentindex(controller->getCountDownMillis() / 1000 - 1);

                    for (int i = 1; i <= 5; i++) {
                        setting->add_values(std::to_string(i) + "s");
                    }
                }

                {
                    auto setting = currentBoothSettings.mutable_update_mode();
                    setting->set_name("Update Mode");
                    setting->set_post_url("/update");
                    setting->set_alert("Do you really want to start the Update Mode?");
                }

                {
                    auto setting = currentBoothSettings.mutable_cups_link();
                    setting->set_name("CUPS Printer Setup");
                    setting->set_url("http://192.168.4.1:631");
                }


                res << currentBoothSettings.SerializeAsString();
            });


    mux.handle("/stress")
            .post([this](served::response &res, const served::request &req) {
                auto controller = logic->getSelfomatController();
                controller->setStressTestEnabled(true);
                served::response::stock_reply(200, res);
            });

    mux.handle("/unstress")
            .post([this](served::response &res, const served::request &req) {
                auto controller = logic->getSelfomatController();
                controller->setStressTestEnabled(false);
                served::response::stock_reply(200, res);
            });

    mux.handle("/version")
            .get([this](served::response &res, const served::request &req) {
                std::string filename = "./version";

                ifstream f(filename, ios::in);
                string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                res.set_status(200);
                res.set_body(file_contents);
            });


    mux.handle("/app/svg/{file}")
            .get([this](served::response &res, const served::request &req) {
                std::string filename = "./app/svg/" + req.params["file"];

                res.set_header("Content-Type", "image/svg+xml");

                ifstream f(filename, ios::in);
                string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                res.set_status(200);
                res.set_body(file_contents);
            });

    mux.handle("/app/{file}")
            .get([this](served::response &res, const served::request &req) {
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