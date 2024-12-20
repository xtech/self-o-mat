//
// Created by clemens on 13.02.19.
//

#include "BoothApi.h"

using namespace selfomat::api;
using namespace xtech::selfomat;
using namespace selfomat::logic;

std::string BoothApi::TAG = "API";

BoothApi::BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera, bool show_led_setup) : logic(logic),
    camera(camera) {
    this->show_led_setup = show_led_setup;
}

bool BoothApi::start() {
    // Register some URLs where the app should be reachable
    // there's surely a better way to do this.
    CROW_ROUTE(app, "/")([]() {
        auto response = crow::response();
        response.set_static_file_info("app/index.html");
        return response;
    });

    CROW_ROUTE(app, "/app")([]() {
        auto response = crow::response();
        response.set_static_file_info("app/index.html");
        return response;
    });
    CROW_ROUTE(app, "/tabs/<path>")([](const std::string &path) {
        (void)path;
        auto response = crow::response();
        response.redirect("/");
        return response;
    });


    CROW_ROUTE(app, "/camera_settings/aperture").methods(crow::HTTPMethod::Post)(
        [this](const crow::request &req) {
            if (camera->getState() != STATE_WORKING) {
                return crow::response(503);
            }

            IntUpdate update;
            if (!update.ParseFromString(req.body)) {
                return crow::response(400);
            }

            camera->setAperture(update.value());

            return crow::response(200);
        });


    CROW_ROUTE(app, "/camera_settings").methods(crow::HTTPMethod::Get)(
        [this](const crow::request &req) {
            if (camera->getState() != STATE_WORKING) {
                return crow::response(503);
            }

            boost::property_tree::ptree locale;
            try {
                boost::property_tree::read_json("./i18n/" + req.get_header_value("lang") + ".json", locale);
            } catch (boost::exception &e) {
                boost::property_tree::read_json("./i18n/en.json", locale);
            }

            CameraSettings currentCameraSettings; {
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
            } {
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
            } {
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
            } {
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
            } {
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
            } {
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
            } {
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


            return crow::response(currentCameraSettings.SerializeAsString());
        });

    CROW_ROUTE(app, "/booth_settings").methods(crow::HTTPMethod::Get)(
        [this](const crow::request &req) {
            BoothSettings currentBoothSettings;

            boost::property_tree::ptree locale;
            try {
                boost::property_tree::read_json("./i18n/" + req.get_header_value("lang") + ".json", locale);
            } catch (boost::exception &e) {
                boost::property_tree::read_json("./i18n/en.json", locale);
            }


            auto controller = logic->getSelfomatController(); {
                auto setting = currentBoothSettings.mutable_language_choice();
                setting->set_name(locale.get<string>("api.booth.languageChoice"));
                setting->set_update_url("/booth_settings/language/which");
                setting->set_currentindex(logic->getLanguageChoice());
                for (auto &choice: *logic->getLanguageChoices())
                    setting->add_values(choice);
            } {
                auto setting = currentBoothSettings.mutable_storage_enabled();
                setting->set_name(locale.get<string>("api.booth.storageEnabled"));
                setting->set_update_url("/booth_settings/storage/enabled");
                setting->set_currentvalue(logic->getStorageEnabled());
            } {
                auto setting = currentBoothSettings.mutable_printer_enabled();
                setting->set_name(locale.get<string>("api.booth.printerEnabled"));
                setting->set_update_url("/booth_settings/printer/enabled");
                setting->set_currentvalue(logic->getPrinterEnabled());
            } {
                auto setting = currentBoothSettings.mutable_print_confirmation_enabled();
                setting->set_name(locale.get<string>("api.booth.printConfirmationEnabled"));
                setting->set_update_url("/booth_settings/print_confirmation/enabled");
                setting->set_currentvalue(logic->getPrintConfirmationEnabled());
            } {
                auto setting = currentBoothSettings.mutable_filter_choice();
                setting->set_name(locale.get<string>("api.booth.filterEnabled"));
                setting->set_update_url("/booth_settings/filter/which");
                setting->set_currentindex(logic->getFilterChoice());
                for (auto &choice: *logic->getFilterChoices())
                    setting->add_values(choice);
            } {
                auto setting = currentBoothSettings.mutable_filter_gain();
                setting->set_name(locale.get<string>("api.booth.filterGain"));
                setting->set_update_url("/booth_settings/filter/gain");
                setting->set_currentvalue(logic->getFilterGain());
                setting->set_minvalue(0.0);
                setting->set_maxvalue(1.0);
            } {
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
                } {
                    auto setting = currentBoothSettings.mutable_flashmode();
                    setting->set_name(locale.get<string>("api.booth.iTTLEnabled"));
                    setting->set_update_url("/booth_settings/flash/ittl/enabled");
                    setting->set_currentvalue(logic->getSelfomatController()->getFlashMode());
                } {
                    auto setting = currentBoothSettings.mutable_flash_duration_micros();
                    setting->set_update_url("/booth_settings/flash/duration");
                    setting->set_name(locale.get<string>("api.booth.flashDuration"));
                    setting->set_currentvalue(controller->getFlashDurationMicros());
                    setting->set_minvalue(0);
                    setting->set_maxvalue(40000);
                } {
                    auto setting = currentBoothSettings.mutable_flash_test();
                    setting->set_name(locale.get<string>("api.booth.flashTest"));
                    setting->set_post_url("/flash");
                }
            } {
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
                } {
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
            } {
                auto setting = currentBoothSettings.mutable_led_offset_clockwise();
                setting->set_post_url("/booth_settings/led_offset_cw");
                setting->set_name(locale.get<string>("api.booth.ledOffset") + " +");
            } {
                auto setting = currentBoothSettings.mutable_led_offset_counter_clockwise();
                setting->set_post_url("/booth_settings/led_offset_ccw");
                setting->set_name(locale.get<string>("api.booth.ledOffset") + " -");
            } {
                auto setting = currentBoothSettings.mutable_countdown_duration();
                setting->set_update_url("/booth_settings/countdown_duration");
                setting->set_name(locale.get<string>("api.booth.countdownDuration"));

                setting->set_currentindex(controller->getCountDownMillis() / 1000 - 1);

                for (int i = 1; i <= 10; i++) {
                    setting->add_values(std::to_string(i) + "s");
                }
            } {
                auto setting = currentBoothSettings.mutable_max_led_brightness();
                setting->set_update_url("/booth_settings/max_led_brightness");
                setting->set_name(locale.get<string>("api.booth.maxLedBrightness"));
                setting->set_minvalue(20);
                setting->set_maxvalue(255);
                setting->set_currentvalue(controller->getMaxLedBrightness());
            } {
                auto setting = currentBoothSettings.mutable_update_mode();
                setting->set_name(locale.get<string>("api.booth.updateMode"));
                setting->set_post_url("/update");
                setting->set_alert(locale.get<string>("api.booth.updateAlert"));
            } {
                auto setting = currentBoothSettings.mutable_cups_link();
                setting->set_name(locale.get<string>("api.booth.cupsSetup"));
                setting->set_url("http://192.168.4.1:631");
            } {
                auto setting = currentBoothSettings.mutable_debug_log_enabled();
                setting->set_update_url("/booth_settings/debug_log_enabled");
                setting->set_name(locale.get<string>("api.booth.debugLogEnabled"));
                setting->set_currentvalue(logic->getDebugLogEnabled());
            } {
                auto triggerCountSetting = currentBoothSettings.mutable_trigger_counter();
                triggerCountSetting->set_name(locale.get<string>("api.booth.triggerCounter"));
                triggerCountSetting->set_value(std::to_string(logic->getTriggerCounter()));
            } {
                auto setting = currentBoothSettings.mutable_software_version();
                setting->set_name(locale.get<string>("api.booth.software_version"));
                setting->set_value("unreleased (Build date + time: " __DATE__ " " __TIME__ ")");
            }


            return crow::response(currentBoothSettings.SerializeAsString());
        });

    CROW_ROUTE(app, "/camera_settings/iso")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    camera->setIso(update.value());

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/camera_settings/shutter_speed")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    camera->setShutterSpeed(update.value());

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/camera_settings/exposure_correction")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    camera->setExposureCorrection(update.value());

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/camera_settings/exposure_correction_trigger")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    camera->setExposureCorrectionTrigger(update.value());

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/camera_settings/image_format")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    camera->setImageFormat(update.value());

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/storage/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setStorageEnabled(update.value(), true);

                    LOG_I(TAG, "updated storage enabled to: ", std::to_string(update.value()));

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/autofocus_before_trigger/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setAutofocusBeforeTrigger(update.value(), true);

                    LOG_I(TAG, "updated autofocus before trigger enabled to: ", std::to_string(update.value()));

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/flash/ittl/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->getSelfomatController()->setFlashMode(update.value());
                    logic->getSelfomatController()->commit();

                    LOG_I(TAG, "updated flash mode to: ", std::to_string(update.value()));

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/printer/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setPrinterEnabled(update.value(), true);

                    LOG_I(TAG, "updated printer enabled to: ", std::to_string(update.value()));

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/print_confirmation/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setPrintConfirmationEnabled(update.value(), true);

                    LOG_I(TAG, "updated print confirmation enabled to: ", std::to_string(update.value()));

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/flash/enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setFlashEnabled(update.value(), true);

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/flash/duration")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    SelfomatController *controller = logic->getSelfomatController();
                    controller->setFlashDurationMicros(update.value());
                    controller->commit();

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/max_led_brightness")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    LOG_I(TAG, "updated max led brightness to:", std::to_string(update.value()));


                    SelfomatController *controller = logic->getSelfomatController();
                    controller->setMaxLedBrightness(static_cast<uint8_t>(update.value()));
                    controller->commit();

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/template_enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    LOG_I(TAG, "updated template enabled to", std::to_string(update.value()));

                    logic->setTemplateEnabled(update.value(), true);

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/debug_log_enabled")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    BoolUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    LOG_I(TAG, "updated debug log enabled to", std::to_string(update.value()));


                    logic->setDebugLogEnabled(update.value(), true);

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/led_offset_cw")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    SelfomatController *controller = logic->getSelfomatController();
                    controller->moveOffsetLeft();
                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/led_offset_ccw")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    SelfomatController *controller = logic->getSelfomatController();
                    controller->moveOffsetRight();
                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/countdown_duration")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    SelfomatController *controller = logic->getSelfomatController();
                    controller->setCountDownMillis((update.value() + 1) * 1000);
                    controller->commit();

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/led_mode")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
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
                            return crow::response(400);
                    }
                    controller->commit();

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/led_count")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
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
                            return crow::response(400);
                    }
                    controller->commit();

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/trigger")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    auto controller = logic->getSelfomatController();
                    controller->remoteTrigger();
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/cancel_print")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    logic->cancelPrint();
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/focus")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    if (camera->getState() != STATE_WORKING) {
                        return crow::response(503);
                    }

                    logic->adjustFocus();
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/flash")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    SelfomatController *controller = logic->getSelfomatController();
                    controller->triggerFlash();

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/update")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    logic->stopForUpdate();
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/template_upload")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    string body = req.body;

                    if (!logic->updateTemplate((void *) body.c_str(), body.size())) {
                        boost::property_tree::ptree locale;
                        try {
                            boost::property_tree::read_json("./i18n/" + req.get_header_value("lang") + ".json", locale);
                        } catch (boost::exception &e) {
                            boost::property_tree::read_json("./i18n/en.json", locale);
                        }

                        BoothError error;
                        error.set_title(locale.get<string>("api.error.noAlphaTitle"));
                        error.set_message(locale.get<string>("api.error.noAlpha"));
                        error.set_code(400);

                        return crow::response(error.SerializeAsString());
                    }

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/booth_settings/language/which")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setLanguageChoice(update.value(), true);

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/filter/which")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    IntUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setFilterChoice(update.value(), true);

                    return crow::response(200);
                });

    CROW_ROUTE(app, "/booth_settings/filter/gain")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    FloatUpdate update;
                    if (!update.ParseFromString(req.body)) {
                        return crow::response(400);
                    }

                    logic->setFilterGain(update.value(), true);

                    return crow::response(200);
                });


    CROW_ROUTE(app, "/stress")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    auto controller = logic->getSelfomatController();
                    controller->setStressTestEnabled(true);
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/unstress")
            .methods(crow::HTTPMethod::Post)(
                [this](const crow::request &req) {
                    auto controller = logic->getSelfomatController();
                    controller->setStressTestEnabled(false);
                    return crow::response(200);
                });

    CROW_ROUTE(app, "/version")
            .methods(crow::HTTPMethod::Get)(
                [this](const crow::request &req) {
                    std::string filename = "./version";

                    ifstream f(filename, ios::in);
                    string file_contents{istreambuf_iterator<char>(f), istreambuf_iterator<char>()};

                    return crow::response(file_contents);
                });


    // register the fallback handler to serve static files
    CROW_ROUTE(app, "/<path>").methods(crow::HTTPMethod::Get)([](std::string file) {
        auto response = crow::response();
        response.set_static_file_info("app/" + file);
        return response;
    });
    CROW_ROUTE(app, "/app/<path>")([](std::string file) {
        auto response = crow::response();
        response.set_static_file_info("app/" + file);
        return response;
    });


    LOG_I(TAG, "API started!");

    app.bindaddr("0.0.0.0");
    app.port(9080);
    app_handle = app.run_async();

    return true;
}

bool BoothApi::stop() {
    LOG_I(TAG, "Stopping the API");
    app.stop();
    app_handle.wait();
    LOG_I(TAG, "API stopped");
    return true;
}
