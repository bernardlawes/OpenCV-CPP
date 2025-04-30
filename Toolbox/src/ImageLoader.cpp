// === File: src/ImageLoader.cpp ===
#include "ImageLoader.hpp"
#include <iostream>

cv::Mat ImageLoader::load(const std::string& path) {
    cv::Mat img = cv::imread(path);
    if (img.empty()) {
        std::cerr << "[ERROR] Could not open or find the image at " << path << "\n";
    }
    return img;
}