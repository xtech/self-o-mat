#include <iostream>
#include <boost/thread.hpp>
#include "ui/BoothGui.h"
#include "logic/BoothLogic.h"
#include "camera/OpenCVCamera.h"
#include "camera/GphotoCamera.h"
#include "camera/NopCamera.h"
#include "api/BoothApi.h"
#include "ui/NopGui.h"
#include <tools/readfile.h>
#include <tools/verbose.h>

#include <sstream>
#include <iostream>

using namespace std;
using namespace boost;


using namespace selfomat;

selfomat::ui::IGui *p_gui = nullptr;
selfomat::camera::ICamera *p_cam = nullptr;
selfomat::api::BoothApi *p_api = nullptr;
selfomat::logic::BoothLogic *p_logic = nullptr;

std::mutex exitMutex;
bool cleaned = false;

std::string TAG = "MAIN";

void exitfunc(int code) {
    exitMutex.lock();
    // Clean up only once
    if (cleaned) {
        exitMutex.unlock();
        return;
    }

    cleaned = true;

    LOG_I(TAG, "starting clean up");

    // First, we stop the api, otherwise p_api->stop() won't get called
    if (p_api != nullptr) {
        p_api->stop();
        delete (p_api);
        p_api = nullptr;
    }

    // Now we stop the logic as it will stop camera and gui for us
    if (p_logic != nullptr) {
        if(!p_logic->isStopped()) {
            LOG_I(TAG, "We need to stop the logic");
            p_logic->stop();
            if (p_gui != nullptr)
                delete (p_gui);
        } else {
            LOG_I(TAG, "Logic was already stopped.");
        }

        if (p_cam != nullptr) {
            delete (p_cam);
            p_cam = nullptr;
        }

        p_gui = nullptr;

        delete (p_logic);
        p_logic = nullptr;
    } else {
        // We have to stop camera and gui ourselves
        if (p_cam != nullptr) {
            p_cam->stop();
            delete (p_cam);
            p_cam = nullptr;
        }
        if (p_gui != nullptr) {
            p_gui->stop();
            delete (p_gui);
            p_gui = nullptr;
        }
    }

    LOG_I("main", "Disable filesystem logger");

    selfomat::tools::FilesystemLogger::INSTANCE.disableLogToFile();

    std::cout << "Cleaned up" << endl;
    exitMutex.unlock();
}

void exitfunc() {
    exitfunc(-1);
}

int main(int argc, char *argv[]) {


    // Listen to some common signals so that we're able to stop the camera gui etc gracefully
    //^C
    signal(SIGINT, exitfunc);
    //abort()
    signal(SIGABRT, exitfunc);
    //sent by "kill" command
    signal(SIGTERM, exitfunc);
    //^Z
    signal(SIGTSTP, exitfunc);

    // Listen for return from main method to clean up as well
    atexit(exitfunc);


    // Read settings for the device we're running on

    string camera_type;
    bool debug = false;
    bool disable_watchdog = false;
    bool has_button = false;
    bool has_flash = false;
    bool show_led_setup = false;
    bool force_image_dir_mountpoint = true;
    bool autofocus_before_trigger = false;
    bool fullscreen = true;
    string button_port_name;
    string image_dir;



    boost::property_tree::ptree ptree;

    string box_type = "box_kit_v1";
    readFile("/opt/.selfomat.type", box_type);
    box_type.erase(std::remove(box_type.begin(), box_type.end(), '\n'), box_type.end());

    LOG_I(TAG, "Loading settings file for box: ", box_type);

    try {
        boost::property_tree::read_json("./settings/" + box_type + ".json", ptree);
        camera_type = ptree.get<string>("camera_type");
        image_dir = ptree.get<string>("image_dir");
        button_port_name = ptree.get<string>("button_port_name");
        debug = ptree.get<bool>("debug");
        disable_watchdog = ptree.get<bool>("disable_watchdog");
        has_button = ptree.get<bool>("has_button");
        has_flash = ptree.get<bool>("has_flash");
        show_led_setup = ptree.get<bool>("show_led_setup");
        force_image_dir_mountpoint = ptree.get<bool>("force_image_dir_mountpoint", true);
        autofocus_before_trigger = ptree.get<bool>("autofocus_before_trigger", false);
        fullscreen = ptree.get<bool>("fullscreen", true);
    } catch (boost::exception &e) {
        LOG_E(TAG, "Error loading properties. Using defaults.");
    }

    LOG_D(TAG, "Using camera: ", camera_type);
    LOG_D(TAG, "Has Button: ", std::to_string(has_button));
    LOG_D(TAG, "Button Port Name: ", button_port_name);
    LOG_D(TAG, "Has Flash: ", std::to_string(has_flash));
    LOG_D(TAG, "Force Imagedir mountpoint: ", std::to_string(force_image_dir_mountpoint));
    LOG_D(TAG, "Autofocus Before Trigger: ", std::to_string(autofocus_before_trigger));
    LOG_D(TAG, "Fullscreen: ", std::to_string(fullscreen));


    // We'll set the controller later when logic is initialized
    BoothGui *boothGuiPtr = new BoothGui(fullscreen, debug, nullptr);
    p_gui = boothGuiPtr;
    if (!p_gui->start()) {
        LOG_E(TAG, "Error starting gui - Exiting.");
        return 1;
    }

    LOG_I(TAG, "Started Gui");

    if (camera_type == "gphoto") {
        p_cam = new gphoto::GphotoCamera();
    } else if (camera_type == "opencv") {
        p_cam = new OpenCVCamera();
    } else {
        p_cam = new NopCamera();
    }

    LOG_I(TAG, "Started Camera");

    p_logic = new logic::BoothLogic(p_cam, p_gui, has_button, button_port_name, has_flash, image_dir, force_image_dir_mountpoint, disable_watchdog, show_led_setup, autofocus_before_trigger);

    LOG_I(TAG, "Started Logic");


    boothGuiPtr->setLogicController(p_logic);

    while(true) {
        try {
            p_api = new api::BoothApi(p_logic, p_cam, show_led_setup);
            LOG_I(TAG, "Starting API");
            p_api->start();
            break;
        } catch (boost::exception &e) {
            LOG_E(TAG, "Error creating api - retrying in 5 sec");
            boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        }
    }

    LOG_I(TAG, "API started");

    if (!p_logic->start()) {
        LOG_E(TAG, "Error starting Logic - Exiting.");
        return 2;
    }

    return p_logic->join();
}