//
// Created by clemens on 13.02.19.
//

#ifndef SELF_O_MAT_BOOTHAPI_H
#define SELF_O_MAT_BOOTHAPI_H

#include "../protobuf/api.pb.h"
#include "../logic/BoothLogic.h"
#include <crow.h>
#include <crow/middlewares/cors.h>


using namespace selfomat;
namespace selfomat {
    namespace api {

        class BoothApi {
        private:
            static std::string TAG;
            selfomat::logic::BoothLogic *logic;
            ICamera *camera;

            crow::App<crow::CORSHandler> app;
            std::future<void> app_handle;

            bool show_led_setup;

        public:
            BoothApi(selfomat::logic::BoothLogic *logic, ICamera *camera, bool show_led_setup);

            bool start();

            bool stop();
        };
    }
}


#endif //SELF_O_MAT_BOOTHAPI_H