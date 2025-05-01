#pragma once
#include <opencv2/opencv.hpp>

struct MorphologyResult {
    cv::Mat blurred;
    cv::Mat edges;
    cv::Mat dilated;
    cv::Mat eroded;
    cv::Mat grayed;
};

class MorphologyProcessor {
public:
    MorphologyResult process(const cv::Mat& image);
};
