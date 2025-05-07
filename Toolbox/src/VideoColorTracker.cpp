#include "VideoColorTracker.hpp"
#include "DisplayManager.hpp"
#include <iostream>

// New constructor definition
VideoColorTracker::VideoColorTracker(const int& cameraid) {
    cameraID = cameraid;
    hmin = 0;
    smin = 0;
    vmin = 0;
    hmax = 255;
    smax = 255;
    vmax = 255;
}


void VideoColorTracker::Load(){

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

void VideoColorTracker::Overlay(const cv::Mat& frame, const cv::Mat& canvas, cv::Mat& overlay){
    add(frame, canvas, overlay);
}

// Visual feedback: draw a green dot at the center
void VideoColorTracker::Marker(cv::Mat& frame, cv::Point center){

    if (center.x >= 0) circle(frame, center, 10, cv::Scalar(0, 255, 0), -1);

}

void VideoColorTracker::Trace(const cv::Mat& mask, cv::Mat& canvas, cv::Point& last_center, bool paint_enabled){

    std::vector<std::vector<cv::Point>> contours; // For storing detected contours
    std::vector<cv::Vec4i> hierarchy;

    findContours(mask, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

    if (!contours.empty()) {
        // Find the largest contour (assumes it's the object we care about)
        size_t largestIdx = 0;
        for (size_t i = 1; i < contours.size(); i++) {
            if (contourArea(contours[i]) > contourArea(contours[largestIdx]))
                largestIdx = i;
        }

        // Compute the center (centroid) of the largest contour
        cv::Moments M = moments(contours[largestIdx]);
        if (M.m00 > 0) {
            cv::Point center(static_cast<int>(M.m10 / M.m00), static_cast<int>(M.m01 / M.m00));

            // Draw a line from the last point to the current point
            if (paint_enabled && last_center.x >= 0)
                line(canvas, last_center, center, cv::Scalar(0, 0, 255), 5);

            last_center = center;

            // Visual feedback: draw a green dot at the center
            //circle(frame, center, 10, cv::Scalar(0, 255, 0), -1);
        }
    } else {
        // If no object detected, lift the "pen" (don't draw)
        last_center = cv::Point(-1, -1);
    }

}


void VideoColorTracker::Clean(cv::Mat& mask){
    erode(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);    // Removes small white noise
    dilate(mask, mask, cv::Mat(), cv::Point(-1, -1), 2);  // Restores eroded parts
}

void VideoColorTracker::Detect(cv::Mat& frame, cv::Mat& hsv, cv::Mat& mask){

    if (!cap.isOpened()) {
        std::cerr << "Camera not opened!" << std::endl;
        return;
    }

    cap.read(frame);

    // Resize and flip horizontally (mirror effect)
    // Flipping creates a mirrored camera effect, like how you'd see yourself in a webcam or mirror. It's common in drawing apps, gesture control, and AR experiences, because:
    // When user moves his/her hand to the right, the dot moves to the right on screen.
    // Without flipping, my movement feels "backward" — right is left, left is right.
    
    resize(frame, frame, cv::Size(640, 480));
    flip(frame, frame, 1);

    cvtColor(frame, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower(hmin, smin, vmin);
    cv::Scalar upper(hmax, smax, vmax);

    inRange(hsv, lower, upper, mask);


}