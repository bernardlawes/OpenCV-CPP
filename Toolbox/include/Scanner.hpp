#pragma once
#include <opencv2/opencv.hpp>

class Scanner {
    public:

        cv::VideoCapture cap;
        int cameraID = 0;
        int hmin = 0;
        int smin = 0;
        int vmin = 0;
        int hmax = 255;
        int smax = 255;
        int vmax = 255;
        std::string windowName = "Trackbars";

        // Constructors
        Scanner() = default;
        Scanner(const int& cameraid);  // New constructor

        // Methods
        void Load();
        cv::Mat GetFrame();
        bool Is_Quad(const std::vector<cv::Point>& approxContour);
        std::vector<cv::Point> Reorder(const std::vector<cv::Point>& points);
        cv::Mat Crop(const cv::Mat& img, float w, float h  );
        cv::Mat Orthogonize(const cv::Mat& img, const std::vector<cv::Point>& iniPoints, float w, float h);
        cv::Mat Warp(cv::Mat img, std::vector<cv::Point> points, float w, float h) ;
        cv::Mat Annotate(const cv::Mat& image, const std::vector<cv::Point>& tarcontour = {});
        std::vector<cv::Point> Trace(const cv::Mat& image);
        cv::Mat Process(const cv::Mat& image);
    };