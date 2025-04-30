#include "MorphologicalProcessor.hpp"

MorphologicalResult MorphologicalProcessor::process(const cv::Mat& image) {
    MorphologicalResult result;

    cv::GaussianBlur(image, result.blurred, cv::Size(5, 5), 1.5);

    cv::Mat gray;
    cv::cvtColor(result.blurred, gray, cv::COLOR_BGR2GRAY);
    cv::Canny(gray, result.edges, 100, 200);

    auto kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::dilate(result.edges, result.dilated, kernel);
    cv::erode(result.dilated, result.eroded, kernel);

    return result;
}
