//
// Created by clemens on 18.06.19.
//

#ifndef SELF_O_MAT_BASICIMAGEFILTER_H
#define SELF_O_MAT_BASICIMAGEFILTER_H


#include "IImageFilter.h"
#include <iostream>

namespace selfomat {
    namespace logic {
        class BasicImageFilter : public IImageFilter {
        public:
            std::string getName() override;

            void processImage(Magick::Image &image, double gain) override;
        };

    }
}


#endif //SELF_O_MAT_BASICIMAGEFILTER_H
