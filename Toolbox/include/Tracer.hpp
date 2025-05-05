#pragma once
/*
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
*/
#include <opencv2/opencv.hpp>
#include <iostream>

struct TraceResult {
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> heirarchy;
    int count;
    cv::Mat markup;
    cv::Mat curated;
    std::vector<std::vector<cv::Point>> approxContours;  // approximated contours | a 2D vector (a vector of vectors) 
    std::vector<cv::Rect> bbox;                          // Bounding Rectangles for each contour
    std::vector<int> areas;                              // areas for each contour
    std::vector<int> perimeters;                         // areas for each contour
    std::vector<int> segments;                           // segment counts for each contour
    std::vector<float> aspects;                          // aspect ratios for each contour
    std::vector<cv::Point> midpoints;                    // aspect ratios for each contour
    std::vector<std::string> shapes;                     // Shape names
};

class Tracer {
    public:
    
        TraceResult Trace(const cv::Mat& image);
        void Filter(TraceResult& input, const std::vector<int>& thresholds);
        std::string GetShape(const std::vector<cv::Point>& approxContour);
        cv::Point GetCentroid(const cv::Rect& bbox);

    };