
#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/opencv.hpp>
#include "Capability.hpp"
#include "ColorDetector.hpp"
#include "DisplayManager.hpp"

class ColorDetectorCapability : public Capability {
public:
    void execute(const cv::Mat& image) override {
        ColorDetector redDetector(cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255));
        cv::Mat mask = redDetector.detect(image);

        DisplayManager display;
        cv::Size sz(640, 480);
        display.show("Original", image, sz, cv::Point(0, 0));
        display.show("Red Mask", mask, sz, cv::Point(0, sz.height + 40));
        display.wait();
    }
};
