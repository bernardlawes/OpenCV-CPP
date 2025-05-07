#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>

class VideoColorTracker {
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
    VideoColorTracker() = default;           // Default constructor
    VideoColorTracker(const int& cameraid);  // New constructor

    // Methods
    void Load();
    void Detect(cv::Mat& img, cv::Mat& hsv, cv::Mat& mask);
    void Clean(cv::Mat& mask);
    void Trace(const cv::Mat& mask, cv::Mat& canvas, cv::Point& last_center, bool paint_enabled);
    void Marker(cv::Mat& frame, cv::Point center);
    void Overlay(const cv::Mat& frame, const cv::Mat& canvas, cv::Mat& overlay);
};
