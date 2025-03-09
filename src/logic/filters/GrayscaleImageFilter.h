//
// Created by maehw on 08.03.2025.
//

#ifndef SELF_O_MAT_GRAYSCALEIMAGEFILTER_H
#define SELF_O_MAT_GRAYSCALEIMAGEFILTER_H


#include "IImageFilter.h"
#include <iostream>

namespace selfomat {
    namespace logic {
        class GrayscaleImageFilter : public IImageFilter {
        public:
            std::string getName() override;

            void processImage(cv::Mat &image, double gain) override;
        };

    }
}


#endif //SELF_O_MAT_GRAYSCALEIMAGEFILTER_H
