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

void exitfunc(int code) {
    exitMutex.lock();
    // Clean up only once
    if (cleaned) {
        exitMutex.unlock();
        return;
    }

    cleaned = true;

    std::cout << "starting clean up" << std::endl;

    // First, we stop the api, otherwise p_api->stop() won't get called
    if (p_api != nullptr) {
        p_api->stop();
        delete (p_api);
        p_api = nullptr;
    }

    // Now we stop the logic as it will stop camera and gui for us
    if (p_logic != nullptr) {
        if(!p_logic->isStopped()) {
            std::cout << "We need to stop the logic" << std::endl;
            p_logic->stop();
            if (p_gui != nullptr)
                delete (p_gui);
        } else {
            std::cout << "Logic was already stopped." << std::endl;
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
    string button_port_name;
    string image_dir;



    boost::property_tree::ptree ptree;

    string box_type = "box_kit_v1";
    readFile("/opt/.selfomat.type", box_type);
    box_type.erase(std::remove(box_type.begin(), box_type.end(), '\n'), box_type.end());

    cout << "Loading settings file for box: " << box_type << endl;

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
    } catch (boost::exception &e) {
        cerr << "Error loading properties. Using defaults." << endl;
    }

    cout << "Using camera: " << camera_type << endl;
    cout << "Has Button: " << has_button << endl;
    cout << "Button Port Name: " << button_port_name << endl;
    cout << "Has Flash: " << has_flash << endl;


    // We'll set the controller later when logic is initialized
    BoothGui *boothGuiPtr = new BoothGui(debug, nullptr);
    p_gui = boothGuiPtr;
    if (!p_gui->start()) {
        cerr << "Error starting gui - Exiting." << endl;
        return 1;
    }

    cout << "Started Gui" << endl;

    if (camera_type == "gphoto") {
        p_cam = new gphoto::GphotoCamera();
    } else if (camera_type == "opencv") {
        p_cam = new OpenCVCamera();
    } else {
        p_cam = new NopCamera();
    }

    cout << "Started Camera" << endl;

    p_logic = new logic::BoothLogic(p_cam, p_gui, has_button, button_port_name, has_flash, image_dir, disable_watchdog, show_led_setup);

    cout << "Started Logic" << endl;

    boothGuiPtr->setLogicController(p_logic);

    while(true) {
        try {
            p_api = new api::BoothApi(p_logic, p_cam, show_led_setup);
            cout << "Starting API" << endl;
            p_api->start();
            break;
        } catch (boost::exception &e) {
            cerr << "Error creqting api - retrying in 5 sec" << endl;
            boost::this_thread::sleep(boost::posix_time::milliseconds(5000));
        }
    }

    cout << "API started" << endl;

    if (!p_logic->start()) {
        cerr << "Error starting Logic - Exiting." << endl;
        return 2;
    }

    return p_logic->join();
}