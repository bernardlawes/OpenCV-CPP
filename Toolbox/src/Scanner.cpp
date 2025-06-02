#include "Scanner.hpp"
#include <iostream>




Scanner::Scanner(const int& cameraid) : cameraID(cameraid) {}
// same as writing:
// Scanner::Scanner(const int& cameraid) {
//    cameraID = cameraid;/
// }

void Scanner::Load(){

    cap.open(cameraID);

    cv::namedWindow(windowName, (200, 640)); //Create Window
	cv::createTrackbar("Hue Min", windowName, &hmin, 179);
	cv::createTrackbar("Hue Max", windowName, &hmax, 179);
	cv::createTrackbar("Sat Min", windowName, &smin, 255);
	cv::createTrackbar("Sat Max", windowName, &smax, 255);
	cv::createTrackbar("Val Min", windowName, &vmin, 255);
	cv::createTrackbar("Val Max", windowName, &vmax, 255);
    cv::moveWindow(windowName,0,700);

}

cv::Mat Scanner::GetFrame() {
    
    cv::Mat frame;
    if (!cap.isOpened()) {
        std::cerr << "Error: Camera not opened." << std::endl;
        return frame; // Return an empty frame
    }

    cap >> frame; // Capture a new frame from the camera
    if (frame.empty()) {
        std::cerr << "Error: Frame is empty." << std::endl;
    }

    return frame;
}

cv::Mat Scanner::Process(const cv::Mat& img){
    
    cv::Mat imgGray, imgBlur, imgCanny, imgDilated, hsv, mask;

    
    cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower(hmin, smin, vmin);
    cv::Scalar upper(hmax, smax, vmax);

    inRange(hsv, lower, upper, mask);

    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    //cv:cvtColor(img, imgGray, cv::COLOR_BGR2GRAY);
    //cv::GaussianBlur(imgGray, imgBlur, cv::Size(3, 3), 3, 0);
    //cv::Canny(imgBlur, imgCanny, 25,75);
    cv::Canny(mask, imgCanny, 25, 75);
    cv::dilate(imgCanny, imgDilated, kernel);


    return imgDilated;

}

bool Scanner::Is_Quad(const std::vector<cv::Point>& approxContour){

        // Get side vectors
        std::vector<double> slopes;
        for (int i = 0; i < 4; ++i) {
            cv::Point p1 = approxContour[i];
            cv::Point p2 = approxContour[(i + 1) % 4];
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;
            slopes.push_back(dx != 0 ? dy / dx : 1e9);  // avoid division by zero
        }

        // Count how many pairs of sides have similar slopes (indicating parallel lines)
        int parallelCount = 0;
        for (int i = 0; i < 2; ++i) {
            double s1 = slopes[i];
            double s2 = slopes[i + 2];
            if (std::abs(s1 - s2) < 0.1) // slope tolerance
                parallelCount++;
        }

        // Bounding rectangle aspect ratio
        cv::Rect boundingRect = cv::boundingRect(approxContour);
        float aspectRatio = static_cast<float>(boundingRect.width) / boundingRect.height;

        if (parallelCount < 1) {
            return false;
        } 
        
        return true;
}   

std::vector<cv::Point> Scanner::Trace(const cv::Mat& img){
    
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    int countourId = -1;
    double minArea = 1000; // Minimum area threshold
    double maxArea = 0;
    int cornerCount = 0; // Number of corners to approximate the contour
    
    // Find contours
    cv::findContours(img, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
    
    // Optional: Draw contours for debugging
    //cv::drawContours(img, contours, countourId, cv::Scalar(255, 0, 0), 2);

    std::vector<cv::Point> approxContour;
    std::vector<cv::Rect>  boundingRects;

    for (const auto& contour : contours) {
        double area = cv::contourArea(contour);
        if (area > minArea) {
            float perimeter = cv::arcLength(contour, true);                    // boolean ensures that the contour is closed or not
            cv::approxPolyDP(contour, approxContour, 0.02 * perimeter, true);  // the smaller the factor, the more sensitive it is to finding points
            cornerCount = approxContour.size();                                // Get the number of corners in the approximated contour

            if (cornerCount != 4) {continue;}                                  // Skip contours that do not have exactly 4 corners }
            if (!Is_Quad(approxContour)) {continue;}                     // Check if the contour is a quadrilateral

            // Store the 4 corners of the contour
            //targetPoints = { approxContour[0], approxContour[1], approxContour[2], approxContour[3] }; 

            // Optional: Draw the approximated contour for debugging
            cv::drawContours(img, std::vector<std::vector<cv::Point>>{approxContour}, -1, cv::Scalar(0, 255, 0), 2);

        }
    }

    return approxContour;

}

std::vector<cv::Point> Scanner::Reorder(const std::vector<cv::Point>& points) {
    
    if (points.size() < 4) {
        std::cerr << "[ERROR] Not enough points to reorder." << std::endl;
        return {};
    }

    std::vector<cv::Point> orderedPoints(4);
    std::vector<int> sumOfPoints, subOfPoints;

    for (const auto& pt : points) {
        sumOfPoints.push_back(pt.x + pt.y);
        subOfPoints.push_back(pt.x - pt.y);
    }

    orderedPoints[0] = points[std::min_element(sumOfPoints.begin(), sumOfPoints.end()) - sumOfPoints.begin()]; // top-left
    orderedPoints[1] = points[std::max_element(subOfPoints.begin(), subOfPoints.end()) - subOfPoints.begin()]; // top-right
    orderedPoints[2] = points[std::min_element(subOfPoints.begin(), subOfPoints.end()) - subOfPoints.begin()]; // bottom-left
    orderedPoints[3] = points[std::max_element(sumOfPoints.begin(), sumOfPoints.end()) - sumOfPoints.begin()]; // bottom-right

    return orderedPoints;

}

cv::Mat Scanner::Warp(cv::Mat img, std::vector<cv::Point> points, float w, float h) {
	
    cv::Mat matrix, imgWarp;;

	cv::Point2f src[4]{ points[0],points[1],points[2],points[3] };
	cv::Point2f dst[4]{ {0.0f,0.0f},{w,0.0f},{0.0f,h},{w,h} };

	// Transformation Matrix
	matrix = cv::getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWarp, matrix, cv::Point(w, h));

	return imgWarp;
}

cv::Mat Scanner::Annotate(const cv::Mat& img, const std::vector<cv::Point>& contour){
    
    int i = 1;
    cv::Mat canvas = img.clone();
    
    // Draw the contours on the image
    cv::drawContours(canvas, std::vector<std::vector<cv::Point>>{contour}, -1, cv::Scalar(0, 255, 0), 2);
    

    for (const auto& point : contour) {
        // Draw circles at each corner
        cv::circle(canvas, point, 5, cv::Scalar(0, 0, 255), -1);
        cv::putText(canvas, cv::format("(%d)", i), point, cv::FONT_HERSHEY_SIMPLEX, 0.5, cv::Scalar(255, 255, 255), 1);
        i++;
    }




    // Display the image with contours (Debugging)
    // cv::imshow("Contours", canvas);

    return canvas;
}


cv::Mat Scanner::Orthogonize(const cv::Mat& img, const std::vector<cv::Point>& iniPoints, float w, float h  ) {
    
    cv::Mat warpedImage;

    // REORDER POINTS
    std::vector<cv::Point> reordered_points = Reorder(iniPoints);

    // Draw the reordered points on the image
    cv::Mat canvas = Annotate(img, reordered_points);
    cv::Mat warped = Warp(canvas, reordered_points, w, h); // Adjust the size as needed
    cv::Mat cropped = Crop(warped, w, h);

    return cropped;
}

cv::Mat Scanner::Crop(const cv::Mat& img, float w, float h  ){

	int buffer = 10;
	cv::Rect roi(buffer, buffer, w - (2 * buffer), h - (2 * buffer));
	cv::Mat cropped = img(roi);

    return cropped;
}






