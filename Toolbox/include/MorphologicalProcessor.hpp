#pragma once
#include <opencv2/opencv.hpp>

struct MorphologicalResult {
    cv::Mat blurred;
    cv::Mat edges;
    cv::Mat dilated;
    cv::Mat eroded;
};

class MorphologicalProcessor {
public:
    MorphologicalResult process(const cv::Mat& image);
};
