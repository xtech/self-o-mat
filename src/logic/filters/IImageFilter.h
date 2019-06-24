//
// Created by clemens on 18.06.19.
//

#ifndef SELF_O_MAT_IIMAGEFILTER_H
#define SELF_O_MAT_IIMAGEFILTER_H

#include <opencv/cv.hpp>

namespace selfomat {
    namespace logic {
        class IImageFilter {
        protected:
            double interpolate(double min, double max, double percentage);
        public:
            virtual std::string getName() = 0;
            virtual void processImage(cv::Mat &image, double gain) = 0;
        };
    }
}


#endif //SELF_O_MAT_IIMAGEFILTER_H
