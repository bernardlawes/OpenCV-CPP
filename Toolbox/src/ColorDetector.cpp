// === File: src/ColorDetector.cpp ===
#include "ColorDetector.hpp"

ColorDetector::ColorDetector(cv::Scalar lower, cv::Scalar upper)
    : lowerBound(lower), upperBound(upper) {
}

cv::Mat ColorDetector::detect(const cv::Mat& input) {
    cv::Mat hsv, mask;
    cv::cvtColor(input, hsv, cv::COLOR_BGR2HSV);
    cv::inRange(hsv, lowerBound, upperBound, mask);
    return mask;
}