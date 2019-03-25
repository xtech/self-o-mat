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

        public:
            BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera);

            bool start();

            bool stop();
        };
    }
}


#endif //SELF_O_MAT_BOOTHAPI_H