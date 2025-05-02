// === File: src/ImageLoader.cpp ===
#include "Annotator.hpp"
#include <iostream>

cv::Mat Annotator::DrawAnnotations(const cv::Mat& img){
    
    cv::Mat annotated = img.clone();

	// Drawing Circles
    // circle(img, location, radius, colorInScalar, thickness, filled);
	circle(annotated, cv::Point(700, 400), 50, cv::Scalar(69, 0, 255));
	circle(annotated, cv::Point(200, 500), 100, cv::Scalar(255, 69, 0), 3);
	circle(annotated, cv::Point(300, 70), 75, cv::Scalar(0, 69, 255), cv::FILLED);

    // Drawing Rectangles
	rectangle(annotated, cv::Point(110, 100), cv::Point(210, 200),cv::Scalar(255,0,0),2);
	rectangle(annotated, cv::Rect(210, 210, 200, 100), cv::Scalar(255, 100, 0), cv::FILLED);

    return annotated;

}