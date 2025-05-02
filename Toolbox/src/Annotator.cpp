// === File: src/ImageLoader.cpp ===
#include "Annotator.hpp"
#include <iostream>

cv::Mat Annotator::DrawAnnotations(const cv::Mat& img){
    
    cv::Mat annotated = img.clone();

	cv::Size sz = annotated.size();
	int channles = annotated.channels();
	int width = sz.width;
	int height = sz.height;
	cv::Point midpoint = cv::Point(static_cast<int>(sz.width/2.0f),static_cast<int>(sz.height/2.0f));
	int baseRadius = static_cast<int>(std::min(midpoint.x,midpoint.y));

	// Drawing Circles
    // circle(img, location, radius, colorInScalar, thickness, filled);
	circle(annotated, cv::Point(midpoint.x, midpoint.y), baseRadius, cv::Scalar(255, 0, 0), 3);
	circle(annotated, cv::Point(midpoint.x, midpoint.y), static_cast<int>(baseRadius/2.0f), cv::Scalar(0,255, 0), 3);
	circle(annotated, cv::Point(midpoint.x, midpoint.y), static_cast<int>(baseRadius/3.0f), cv::Scalar(0, 0, 255), cv::FILLED);

    // Drawing Rectangles (2 methods)
	rectangle(annotated, cv::Point(100, 100), cv::Point(midpoint.x, 300),cv::Scalar(255,0,0),3);
	rectangle(annotated, cv::Rect(10, 10, 200, 100), cv::Scalar(0, 255, 0), cv::FILLED);

	// Drawing Lines
	line(annotated, cv::Point(0, midpoint.y), cv::Point(width-1, midpoint.y), cv::Scalar(0, 0, 0), 4);

	// Type Text
	putText(annotated, std::to_string(channles)+" Channels", cv::Point(midpoint.x, midpoint.y+100), cv::FONT_HERSHEY_DUPLEX, 3.0, cv::Scalar(0, 0, 0), 3);

    return annotated;

}