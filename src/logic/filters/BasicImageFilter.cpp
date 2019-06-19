//
// Created by clemens on 18.06.19.
//

#include "BasicImageFilter.h"

using namespace selfomat::logic;

void BasicImageFilter::processImage(Magick::Image &image, double gain) {
    image.normalize();
    image.autoGamma();

    image.sigmoidalContrast(0, interpolate(1.0, 2.5, gain));

    image.modulate(interpolate(100.0, 120.0, gain),
            interpolate(100.0, 120.0, gain),
            100.0);
}

std::string BasicImageFilter::getName() {
    return "basic_image_filter";
}
