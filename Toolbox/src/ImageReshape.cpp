// === File: src/ImageLoader.cpp ===
#include "ImageReshape.hpp"
#include <iostream>

cv::Mat ImageReshape::resizer(const cv::Mat& img) {

    if (img.empty()) {
        std::cerr << "[ERROR] Image is empty / invalid\n";
    }

    cv::Mat processed;
    cv::Size dims = cv::Size(200,200);
    cv::resize(img, processed, dims);

    return processed;
}

cv::Mat ImageReshape::rescale(const cv::Mat& img){

    if (img.empty()) {
        std::cerr << "[ERROR] Image is empty / invalid\n";
    }

    cv::Mat processed;
    float scale = 0.3;
    cv::resize(img, processed,cv::Size(),scale,scale);

    return processed;
}

cv::Mat ImageReshape::cropper(const cv::Mat& img){
    if (img.empty()) {
        std::cerr << "[ERROR] Image is empty / invalid\n";
    }

    cv::Mat processed;
    cv::Rect aoi (880,1200,1400,500);
    processed = img(aoi);

    return processed;
}