//
// Created by clemens on 18.06.19.
//

#include "IImageFilter.h"

double selfomat::logic::IImageFilter::interpolate(double min, double max, double percentage) {
    return (max - min)*percentage + min;
}

