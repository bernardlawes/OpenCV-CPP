#pragma once
#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <string>
#include <vector>

class FaceDetector {
public:
    std::string haarfile;
    std::vector<cv::Rect> faces;
    cv::CascadeClassifier faceCascade;
    cv::Mat image;
    cv::Mat markup;

    // Constructors
    FaceDetector() = default;
    FaceDetector(const std::string& haarFilename);  // New constructor

    // Methods
    bool LoadHaarCascade(const std::string& filename);
    int DetectFaces(const cv::Mat& image);
    void DrawBBOX();
    cv::Mat CropFace(int id);
};
