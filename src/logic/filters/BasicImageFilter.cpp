//
// Created by clemens on 18.06.19.
//

#include "BasicImageFilter.h"

using namespace selfomat::logic;

void BasicImageFilter::processImage(cv::Mat &image, double gain) {

    float colorFactor = static_cast<float>(interpolate(1.0, 1.2, gain));
    float historyClipFactor = static_cast<float>(interpolate(0.0, 4.0, gain));


    const int histogramSize = 256;
    double  alpha = 0, beta = 0, minGray = 0, maxGray = 0;
    cv::Mat gray;

    // Convert the image to gray
    cv::cvtColor(image, gray, cv::COLOR_RGB2GRAY);

    if (historyClipFactor == 0.0) {
        // Find min and max value
        cv::minMaxLoc(gray, &minGray, &maxGray);
    } else {
        cv::Mat hist;

        float range[] = { 0, 256 };
        const float* histRange = { range };
        bool uniform = true;
        bool accumulate = false;
        cv::calcHist(&gray, 1, nullptr, cv::Mat(), hist, 1, &histogramSize, &histRange, uniform, accumulate);

        std::vector<float> accumulator(histogramSize);
        accumulator[0] = hist.at<float>(0);
        for (int i = 1; i < histogramSize; i++)
        {
            accumulator[i] = accumulator[i - 1] + hist.at<float>(i);
        }

        float max = accumulator.back();
        historyClipFactor *= (max / 100.0);
        historyClipFactor /= 2.0;
        // locate left cut
        minGray = 0;
        while (accumulator[minGray] < historyClipFactor)
            minGray++;

        // locate right cut
        maxGray = histogramSize - 1;
        while (accumulator[maxGray] >= (max - historyClipFactor))
            maxGray--;
    }

    // current range
    float inputRange = maxGray - minGray;

    alpha = (histogramSize - 1) / inputRange;
    beta = -minGray * alpha;

    image.convertTo(image, -1, alpha, beta);

    // color correction
    cv::cvtColor(image, image, cv::COLOR_RGB2HSV);
    std::vector<cv::Mat> channels;
    cv::split(image, channels);
    channels[1].convertTo(channels[1], -1, colorFactor);
    cv::merge(channels, image);
    cv::cvtColor(image, image, cv::COLOR_HSV2RGB);

}

std::string BasicImageFilter::getName() {
    return "basic_image_filter";
}



