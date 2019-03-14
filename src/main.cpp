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


IGui *gui = nullptr;
ICamera *cam = nullptr;
BoothApi *api = nullptr;
BoothLogic *logic = nullptr;

void exitfunc(int code) {
    std::cout << "starting clean up" << std::endl;
    // First, we stop the logic as it will stop camera and gui for us
    if(logic != nullptr) {
        std::cout << "stopping logic" << std::endl;
        logic->stop();
        delete(cam);
        delete(gui);
    } else {
        // We have to stop camera and gui ourselves
        if(cam != nullptr) {
            cam->stop();
            delete(cam);
        }
        if(gui != nullptr) {
            gui->stop();
            delete(gui);
        }
    }

    // Finally, stop the api
    if(api != nullptr) {
        api->stop();
        delete(api);
    }

    std::cout << "Cleaned up" << endl;
}

void exitfunc() {
    exitfunc(-1);
}

int main(int argc, char* argv[]) {


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
        has_button = ptree.get<bool>("has_button");
        has_flash = ptree.get<bool>("has_flash");
    } catch (boost::exception &e) {
        cerr << "Error loading properties. Using defaults." << endl;
    }

    cout << "Using camera: " << camera_type << endl;
    cout << "Has Button: " << has_button << endl;
    cout << "Button Port Name: " << button_port_name << endl;
    cout << "Has Flash: " << has_flash << endl;



    gui = new BoothGui();
    if(!gui->start()) {
        cerr << "Error starting gui - Exiting." << endl;
        return 1;
    }

    if(camera_type == "gphoto") {
        cam = new GphotoCamera();
    } else if(camera_type == "opencv") {
        cam = new OpenCVCamera();
    } else {
        cam = new NopCamera();
    }

    logic = new BoothLogic(cam, gui, has_button, button_port_name, has_flash, printer_name, image_dir);

    api = new BoothApi(logic, cam);
    api->start();


    if(!logic->start()) {
        cerr << "Error starting Logic - Exiting." << endl;
        return 2;
    }


    cout << "Api started" << endl;

    logic->join();
}