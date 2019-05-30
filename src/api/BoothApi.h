//
// Created by clemens on 13.02.19.
//

#ifndef SELF_O_MAT_BOOTHAPI_H
#define SELF_O_MAT_BOOTHAPI_H

#include "../protobuf/api.pb.h"
#include "../logic/BoothLogic.h"

#include <served/served.hpp>

using namespace selfomat;
namespace selfomat {
    namespace api {

        class BoothApi {
        private:
            selfomat::logic::BoothLogic *logic;
            ICamera *camera;

            served::multiplexer mux;
            served::net::server server;

            bool show_led_setup;

            const std::map<std::string, std::string> headers = {
                    {"Access-Control-Allow-Origin", "*"},
                    {"Access-Control-Allow-Credentials", "true"},
                    {"Access-Control-Allow-Methods", "GET,HEAD,OPTIONS,POST,PUT"},
                    {"Access-Control-Allow-Headers", "Access-Control-Allow-Headers, Origin,Accept, X-Requested-With, Content-Type, Access-Control-Request-Method, Access-Control-Request-Headers"},

            };

        public:
            BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera, bool show_led_setup);

            bool start();

            bool stop();
        };
    }
}


#endif //SELF_O_MAT_BOOTHAPI_H