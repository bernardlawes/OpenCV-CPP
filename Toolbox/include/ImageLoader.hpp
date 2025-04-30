#pragma once
#include <opencv2/opencv.hpp>
#include <string>

class ImageLoader {
public:
    cv::Mat load(const std::string& path);
};