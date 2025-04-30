
#pragma once
#include <opencv2/opencv.hpp>

class ColorDetector {
public:
    ColorDetector(cv::Scalar lower, cv::Scalar upper);
    cv::Mat detect(const cv::Mat& input);

private:
    cv::Scalar lowerBound, upperBound;
};