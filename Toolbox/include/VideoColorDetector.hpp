#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>

class VideoColorDetector {
public:

    cv::VideoCapture cap;
    int cameraID;
    int hmin = 0;
    int smin = 0;
    int vmin = 0;
    int hmax = 255;
    int smax = 255;
    int vmax = 255;
    std::string windowName = "Trackbars";

    // Constructors
    VideoColorDetector() = default;
    VideoColorDetector(const int& cameraid);  // New constructor

    // Methods
    void Load();
    void Detect(cv::Mat& img, cv::Mat& hsv, cv::Mat& mask);
};
