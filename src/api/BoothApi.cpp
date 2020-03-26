//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;
using namespace xtech::selfomat;
using namespace selfomat::logic;

std::string BoothApi::TAG = "API";

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

                boost::property_tree::ptree locale;
                try {
                    boost::property_tree::read_json("./i18n/" + req.header("lang") + ".json", locale);
                } catch (boost::exception &e) {
                    boost::property_tree::read_json("./i18n/en.json", locale);
                }

                CameraSettings currentCameraSettings;
                {
                    auto setting = currentCameraSettings.mutable_aperture();
                    setting->set_update_url("/camera_settings/aperture");
                    setting->set_name(locale.get<string>("api.camera.aperture"));

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
                    setting->set_name(locale.get<string>("api.camera.shutterSpeed"));
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
                    setting->set_name(locale.get<string>("api.camera.liveBrightness"));
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
                    setting->set_name(locale.get<string>("api.camera.captureBrightness"));
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
                    setting->set_name(locale.get<string>("api.camera.imageFormat"));
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
                    setting->set_name(locale.get<string>("api.camera.autoFocus"));
                    setting->set_post_url("/focus");
                }

                auto lensNameSetting = currentCameraSettings.mutable_lens_name();
                lensNameSetting->set_name(locale.get<string>("api.camera.lensName"));
                lensNameSetting->set_value(camera->getLensName());
                auto cameraNameSetting = currentCameraSettings.mutable_camera_name();
                cameraNameSetting->set_name(locale.get<string>("api.camera.cameraName"));
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

                LOG_I(TAG, "updated storage enabled to: ", std::to_string(update.value()));

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings/autofocus_before_trigger/enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setAutofocusBeforeTrigger(update.value(), true);

                LOG_I(TAG, "updated autofocus before trigger enabled to: ", std::to_string(update.value()));

                served::response::stock_reply(200, res);
            });



    mux.handle("/booth_settings/flash/ittl/enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->getSelfomatController()->setFlashMode(update.value());
                logic->getSelfomatController()->commit();

                LOG_I(TAG, "updated flash mode to: ", std::to_string(update.value()));

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

                LOG_I(TAG, "updated printer enabled to: ", std::to_string(update.value()));

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

    mux.handle("/booth_settings/max_led_brightness")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                LOG_I(TAG, "updated max led brightness to:", std::to_string(update.value()));


                SelfomatController *controller = logic->getSelfomatController();
                controller->setMaxLedBrightness(static_cast<uint8_t>(update.value()));
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

                LOG_I(TAG, "updated template enabled to", std::to_string(update.value()));

                logic->setTemplateEnabled(update.value(), true);

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings/debug_log_enabled")
            .post([this](served::response &res, const served::request &req) {
                BoolUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                LOG_I(TAG, "updated debug log enabled to", std::to_string(update.value()));


                logic->setDebugLogEnabled(update.value(), true);

                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/led_offset_cw")
            .post([this](served::response &res, const served::request &req) {

                SelfomatController *controller = logic->getSelfomatController();
                controller->moveOffsetLeft();
                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/led_offset_ccw")
            .post([this](served::response &res, const served::request &req) {

                SelfomatController *controller = logic->getSelfomatController();
                controller->moveOffsetRight();
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

                    boost::property_tree::ptree locale;
                    try {
                        boost::property_tree::read_json("./i18n/" + req.header("lang") + ".json", locale);
                    } catch (boost::exception &e) {
                        boost::property_tree::read_json("./i18n/en.json", locale);
                    }

                    BoothError error;
                    error.set_title(locale.get<string>("api.error.noAlphaTitle"));
                    error.set_message(locale.get<string>("api.error.noAlpha"));
                    error.set_code(400);

                    res << error.SerializeAsString();

                    return;
                }

                served::response::stock_reply(200, res);
            });


    mux.handle("/booth_settings/filter/which")
            .post([this](served::response &res, const served::request &req) {
                IntUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setFilterChoice(update.value(), true);

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings/filter/gain")
            .post([this](served::response &res, const served::request &req) {
                FloatUpdate update;
                if (!update.ParseFromString(req.body())) {
                    served::response::stock_reply(400, res);
                    return;
                }

                logic->setFilterGain(update.value(), true);

                served::response::stock_reply(200, res);
            });

    mux.handle("/booth_settings")
            .get([this](served::response &res, const served::request &req) {
                BoothSettings currentBoothSettings;

                boost::property_tree::ptree locale;
                try {
                    boost::property_tree::read_json("./i18n/" + req.header("lang") + ".json", locale);
                } catch (boost::exception &e) {
                    boost::property_tree::read_json("./i18n/en.json", locale);
                }

                auto controller = logic->getSelfomatController();
                {
                    auto setting = currentBoothSettings.mutable_storage_enabled();
                    setting->set_name(locale.get<string>("api.booth.storageEnabled"));
                    setting->set_update_url("/booth_settings/storage/enabled");
                    setting->set_currentvalue(logic->getStorageEnabled());
                }

                {
                    auto setting = currentBoothSettings.mutable_printer_enabled();
                    setting->set_name(locale.get<string>("api.booth.printerEnabled"));
                    setting->set_update_url("/booth_settings/printer/enabled");
                    setting->set_currentvalue(logic->getPrinterEnabled());
                }

                {
                    auto setting = currentBoothSettings.mutable_filter_choice();
                    setting->set_name(locale.get<string>("api.booth.filterEnabled"));
                    setting->set_update_url("/booth_settings/filter/which");
                    setting->set_currentindex(logic->getFilterChoice());
                    for(auto &choice : *logic->getFilterChoices())
                        setting->add_values(choice);
                }

                {
                    auto setting = currentBoothSettings.mutable_filter_gain();
                    setting->set_name(locale.get<string>("api.booth.filterGain"));
                    setting->set_update_url("/booth_settings/filter/gain");
                    setting->set_currentvalue(logic->getFilterGain());
                    setting->set_minvalue(0.0);
                    setting->set_maxvalue(1.0);
                }

                {
                    auto setting = currentBoothSettings.mutable_autofocus_before_trigger();
                    setting->set_name(locale.get<string>("api.booth.autofocus_before_trigger"));
                    setting->set_update_url("/booth_settings/autofocus_before_trigger/enabled");
                    setting->set_currentvalue(logic->getAutofocusBeforeTrigger());
                }



                bool flashAvailable = logic->getFlashAvailable();
                bool flashEnabled = logic->getFlashEnabled();

                if (flashAvailable) {
                    {
                        auto setting = currentBoothSettings.mutable_flash_enabled();
                        setting->set_name(locale.get<string>("api.booth.flashEnabled"));
                        setting->set_update_url("/booth_settings/flash/enabled");
                        setting->set_currentvalue(flashEnabled);
                    }

                    {
                        auto setting = currentBoothSettings.mutable_flashmode();
                        setting->set_name(locale.get<string>("api.booth.iTTLEnabled"));
                        setting->set_update_url("/booth_settings/flash/ittl/enabled");
                        setting->set_currentvalue(logic->getSelfomatController()->getFlashMode());
                    }

                    {
                        auto setting = currentBoothSettings.mutable_flash_duration_micros();
                        setting->set_update_url("/booth_settings/flash/duration");
                        setting->set_name(locale.get<string>("api.booth.flashDuration"));
                        setting->set_currentvalue(controller->getFlashDurationMicros());
                        setting->set_minvalue(0);
                        setting->set_maxvalue(40000);
                    }

                    {
                        auto setting = currentBoothSettings.mutable_flash_test();
                        setting->set_name(locale.get<string>("api.booth.flashTest"));
                        setting->set_post_url("/flash");
                    }
                }

                {
                        auto setting = currentBoothSettings.mutable_template_upload();
                        setting->set_post_url("/template_upload");
                        setting->set_name(locale.get<string>("api.booth.templateUpload"));
                        setting->set_input_accept("image/x-png,image/png");
                }


                if (logic->getTemplateLoaded()) {
                    {
                        auto setting = currentBoothSettings.mutable_template_enabled();
                        setting->set_update_url("/booth_settings/template_enabled");
                        setting->set_name(locale.get<string>("api.booth.templateEnabled"));
                        setting->set_currentvalue(logic->getTemplateEnabled());
                    }
                }

                if (this->show_led_setup) {
                    {
                        auto setting = currentBoothSettings.mutable_led_mode();
                        setting->set_update_url("/booth_settings/led_mode");
                        setting->set_name(locale.get<string>("api.booth.ledMode"));
                        setting->set_currentindex(controller->getLedType());

                        setting->add_values(SelfomatController::LED_TYPE::RGB.humanName);
                        setting->add_values(SelfomatController::LED_TYPE::RGBW.humanName);
                    }

                    {
                        auto setting = currentBoothSettings.mutable_led_count();
                        setting->set_update_url("/booth_settings/led_count");
                        setting->set_name(locale.get<string>("api.booth.ledCount"));


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
                    setting->set_name(locale.get<string>("api.booth.ledOffset") + " +");
                }

                {
                    auto setting = currentBoothSettings.mutable_led_offset_counter_clockwise();
                    setting->set_post_url("/booth_settings/led_offset_ccw");
                    setting->set_name(locale.get<string>("api.booth.ledOffset") + " -");
                }


                {
                    auto setting = currentBoothSettings.mutable_countdown_duration();
                    setting->set_update_url("/booth_settings/countdown_duration");
                    setting->set_name(locale.get<string>("api.booth.countdownDuration"));

                    setting->set_currentindex(controller->getCountDownMillis() / 1000 - 1);

                    for (int i = 1; i <= 10; i++) {
                        setting->add_values(std::to_string(i) + "s");
                    }
                }

                {
                    auto setting = currentBoothSettings.mutable_max_led_brightness();
                    setting->set_update_url("/booth_settings/max_led_brightness");
                    setting->set_name(locale.get<string>("api.booth.maxLedBrightness"));
                    setting->set_minvalue(20);
                    setting->set_maxvalue(255);
                    setting->set_currentvalue(controller->getMaxLedBrightness());
                }


                {
                    auto setting = currentBoothSettings.mutable_update_mode();
                    setting->set_name(locale.get<string>("api.booth.updateMode"));
                    setting->set_post_url("/update");
                    setting->set_alert(locale.get<string>("api.booth.updateAlert"));
                }

                {
                    auto setting = currentBoothSettings.mutable_cups_link();
                    setting->set_name(locale.get<string>("api.booth.cupsSetup"));
                    setting->set_url("http://192.168.4.1:631");
                }

                {
                    auto setting = currentBoothSettings.mutable_debug_log_enabled();
                    setting->set_update_url("/booth_settings/debug_log_enabled");
                    setting->set_name(locale.get<string>("api.booth.debugLogEnabled"));
                    setting->set_currentvalue(logic->getDebugLogEnabled());
                }

                {
                    auto triggerCountSetting = currentBoothSettings.mutable_trigger_counter();
                    triggerCountSetting->set_name(locale.get<string>("api.booth.triggerCounter"));
                    triggerCountSetting->set_value(std::to_string(logic->getTriggerCounter()));
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

    mux.handle("/app/assets/i18n/{file}")
            .get([this](served::response &res, const served::request &req) {
                std::string filename = "./app/assets/i18n/" + req.params["file"];

                res.set_header("Content-Type", "application/json");

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


    LOG_I(TAG, "API started!");


    return true;
}

bool BoothApi::stop() {
    LOG_I(TAG, "Stopping the API");

    server.stop();

    return true;
}