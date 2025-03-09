//
// Created by maehw on 08.03.2025
//

#include "GrayscaleImageFilter.h"

using namespace selfomat::logic;

void GrayscaleImageFilter::processImage(cv::Mat &image, double gain) {
    // Ignore the gain parameter
    (void)gain;

    // Convert the color image to grayscale
    cv::Mat grayImage;
    cv::cvtColor(image, grayImage, cv::COLOR_BGR2GRAY);

    // Clip pixel values to the valid range [0, 255]
    cv::threshold(grayImage, grayImage, 255, 255, cv::THRESH_TRUNC);

    // Convert grayscale image back to 3-channel format to maintain consistency
    cv::cvtColor(grayImage, image, cv::COLOR_GRAY2BGR);
}

std::string GrayscaleImageFilter::getName() {
    return "grayscale_image_filter";
}
