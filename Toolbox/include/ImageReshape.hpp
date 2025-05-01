#pragma once
#include <opencv2/opencv.hpp>
#include <string>

struct ImageReshapeResult {
    cv::Mat resized;
    cv::Mat rescaled;
    cv::Mat cropped;
};

class ImageReshape {
public:
    cv::Mat resizer(const cv::Mat& image);
    cv::Mat rescale(const cv::Mat& image);
    cv::Mat cropper(const cv::Mat& image);
};