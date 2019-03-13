//
// Created by clemens on 13.02.19.
//

#ifndef SELF_O_MAT_BOOTHAPI_H
#define SELF_O_MAT_BOOTHAPI_H

#include "protobuf/api.pb.h"
#include "BoothLogic.h"

#include <pistache/endpoint.h>
#include <pistache/router.h>


using namespace selfomat;
using namespace Pistache;

class BoothApi {
private:
    BoothLogic *logic;
    ICamera *camera;
    Http::Endpoint httpEndpoint;

    void getCameraChoices(const Rest::Request &request, Http::ResponseWriter response);
    void getCurrentCameraSettings(const Rest::Request &request, Http::ResponseWriter response);

    void setCameraSettings(const Rest::Request &request, Http::ResponseWriter response);

    void triggerCapture(const Rest::Request &request, Http::ResponseWriter response);
    void autofocus(const Rest::Request &request, Http::ResponseWriter response);


public:
    BoothApi(BoothLogic *logic, ICamera *camera);

    bool start();
    bool stop();
};


#endif //SELF_O_MAT_BOOTHAPI_H
