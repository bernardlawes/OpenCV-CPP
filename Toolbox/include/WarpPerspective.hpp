#pragma once
#include <opencv2/opencv.hpp>

class WarpPerspective {
    public:
        cv::Mat Annotate(const cv::Mat& image);
        cv::Mat Orthogonize(const cv::Mat& image);
    };