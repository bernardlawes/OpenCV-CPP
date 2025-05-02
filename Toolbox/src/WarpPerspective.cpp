#include "WarpPerspective.hpp"
#include <iostream>

cv::Mat WarpPerspective::Annotate(const cv::Mat& img){
    
    
    cv::Point2f src[4]{ {578,411},{963,454},{478,881},{907,961} };
    cv::Mat annotated = img.clone();

    // Draw circles at corners on original point
	for (int i = 0; i < 4; i++)
	{
		circle(annotated, src[i], 50, cv::Scalar(0, 0, 255), cv::FILLED);
        putText(annotated, std::to_string(i+1), cv::Point(src[i].x-15,src[i].y+15), cv::FONT_HERSHEY_DUPLEX, 1.5, cv::Scalar(0, 255, 255), 1);
	}

    return annotated;

}

cv::Mat WarpPerspective::Orthogonize(const cv::Mat& img){
    
    cv::Point2f src[4]{ {578,411},{963,454},{478,881},{907,961} };

    cv::Mat matrix, warped;
    
	float w = 250, h = 350;
    
	cv::Point2f dst[4]{ {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

    // Transformation Matrix
	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, warped, matrix, cv::Point(w, h));

    return warped;

}