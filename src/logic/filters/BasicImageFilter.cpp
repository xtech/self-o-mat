//
// Created by clemens on 18.06.19.
//

#include "BasicImageFilter.h"

using namespace selfomat::logic;

void BasicImageFilter::processImage(Magick::Image &image, double gain) {
    std::cout << "filter normalize" << std::endl;
    image.normalize();
    std::cout << "filter gamma" << std::endl;
    image.autoGamma();

    std::cout << "filter contrast" << std::endl;

    image.sigmoidalContrast(0, interpolate(1.0, 2.5, gain));

    std::cout << "filter modulate" << std::endl;

    image.modulate(interpolate(100.0, 120.0, gain),
            interpolate(100.0, 120.0, gain),
            100.0);
    std::cout << "filter done" << std::endl;

}

std::string BasicImageFilter::getName() {
    return "basic_image_filter";
}



