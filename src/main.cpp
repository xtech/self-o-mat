#include <iostream>
#include <boost/thread.hpp>
#include "ui/BoothGui.h"
#include "logic/BoothLogic.h"
#include "camera/OpenCVCamera.h"
#include "camera/GphotoCamera.h"
#include "camera/NopCamera.h"
#include "api/BoothApi.h"
#include "ui/NopGui.h"

using namespace std;
using namespace boost;


using namespace selfomat;

selfomat::ui::IGui *p_gui = nullptr;
selfomat::camera::ICamera *p_cam = nullptr;
selfomat::api::BoothApi *p_api = nullptr;
selfomat::logic::BoothLogic *p_logic = nullptr;


void exitfunc(int code) {

    // Clean up only once
    static bool cleaned = false;
    if (cleaned)
        return;

    std::cout << "starting clean up" << std::endl;

    // First, we stop the api, otherwise p_api->stop() won't get called
    if (p_api != nullptr) {
        p_api->stop();
        delete (p_api);
    }

    // Now we stop the logic as it will stop camera and gui for us
    if (p_logic != nullptr) {
        p_logic->stop();
        delete (p_cam);
        delete (p_gui);
    } else {
        // We have to stop camera and gui ourselves
        if (p_cam != nullptr) {
            p_cam->stop();
            delete (p_cam);
        }
        if (p_gui != nullptr) {
            p_gui->stop();
            delete (p_gui);
        }
    }

    std::cout << "Cleaned up" << endl;
    cleaned = true;
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
    string button_port_name;
    string printer_name;
    string image_dir;

    boost::property_tree::ptree ptree;

    try {
        boost::property_tree::read_json("./settings.json", ptree);
        camera_type = ptree.get<string>("camera_type");
        printer_name = ptree.get<string>("printer_name");
        image_dir = ptree.get<string>("image_dir");
        button_port_name = ptree.get<string>("button_port_name");
        debug = ptree.get<bool>("debug");
        disable_watchdog = ptree.get<bool>("disable_watchdog");
        has_button = ptree.get<bool>("has_button");
        has_flash = ptree.get<bool>("has_flash");
    } catch (boost::exception &e) {
        cerr << "Error loading properties. Using defaults." << endl;
    }

    cout << "Using camera: " << camera_type << endl;
    cout << "Has Button: " << has_button << endl;
    cout << "Button Port Name: " << button_port_name << endl;
    cout << "Has Flash: " << has_flash << endl;


    p_gui = new BoothGui(debug);
    if (!p_gui->start()) {
        cerr << "Error starting gui - Exiting." << endl;
        return 1;
    }

    if (camera_type == "gphoto") {
        p_cam = new gphoto::GphotoCamera();
    } else if (camera_type == "opencv") {
        p_cam = new OpenCVCamera();
    } else {
        p_cam = new NopCamera();
    }

    p_logic = new logic::BoothLogic(p_cam, p_gui, has_button, button_port_name, has_flash, printer_name, image_dir, disable_watchdog);

    p_api = new api::BoothApi(p_logic, p_cam);
    p_api->start();

    cout << "starting logic" << endl;
    if (!p_logic->start()) {
        cerr << "Error starting Logic - Exiting." << endl;
        return 2;
    }

    return p_logic->join();
}