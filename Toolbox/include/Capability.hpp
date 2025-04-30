// Capability.h
#pragma once
#include <opencv2/opencv.hpp>

class Capability {
public:
    virtual ~Capability() = default;
    virtual void execute(const cv::Mat& image) = 0;
};