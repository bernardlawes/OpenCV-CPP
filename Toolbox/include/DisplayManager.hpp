// === File: include/DisplayManager.hpp ===
#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <optional>

class DisplayManager {
public:
    void show(const std::string& windowName, const cv::Mat& image, std::optional<cv::Size> dimensions, std::optional<cv::Point> location);
    void wait(int delayMs = 0);
};