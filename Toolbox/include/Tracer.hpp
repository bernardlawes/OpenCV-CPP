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
    std::vector<std::vector<cv::Point>> filteredContours; // filtered contours after applying thresholds
    std::vector<int> filteredContourIndices; // indices of approved contours in the original image
    std::vector<cv::Vec4i> heirarchy;
    int meanBrightness; // mean brightness of the image
    int count;
    cv::Mat image;
    //cv::Mat filtered;
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
        std::string Inspect_Quadrilateral(const std::vector<cv::Point>& approxContour);
        cv::Point GetCentroid(const cv::Rect& bbox);

    };