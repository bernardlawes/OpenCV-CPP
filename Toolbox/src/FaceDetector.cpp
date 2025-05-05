#include "FaceDetector.hpp"
#include <iostream>

// New constructor definition
FaceDetector::FaceDetector(const std::string& haarFilename) {
    LoadHaarCascade(haarFilename);
}

bool FaceDetector::LoadHaarCascade(const std::string& filename) {
    haarfile = filename;
    if (!faceCascade.load(haarfile)) {
        std::cerr << "Error: Could not load Haar cascade from: " << haarfile << std::endl;
        return false;
    }
    return true;
}

int FaceDetector::DetectFaces(const cv::Mat& img) {
    if (img.empty()) {
        std::cerr << "Image is requird as input" << std::endl;
        return -1;
    }

    cv::Mat gray;
    image = img.clone();

    cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
    cv::equalizeHist(gray, gray);

    faceCascade.detectMultiScale(gray, faces);


    if (faces.empty()) return 0;

    else {
        return faces.size();
    }
}

void FaceDetector::DrawBBOX() {
    markup = image.clone();
    for (const auto& face : faces) {
        cv::rectangle(markup, face, cv::Scalar(0, 255, 0), 2);
    }
}

cv::Mat FaceDetector::CropFace(int id) {
    if (id < 0 || id >= static_cast<int>(faces.size())) {
        std::cerr << "Error: Invalid face ID" << std::endl;
        return cv::Mat();
    }
    return image(faces[id]).clone();
}

// ... rest unchanged
