#pragma once
#include <opencv2/opencv.hpp>

class Annotator {
    public:
        cv::Mat DrawAnnotations(const cv::Mat& image);
    };