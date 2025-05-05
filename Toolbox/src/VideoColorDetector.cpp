#include "VideoColorDetector.hpp"
#include "DisplayManager.hpp"
#include <iostream>

// New constructor definition
VideoColorDetector::VideoColorDetector(const int& cameraid) {
    cameraID = cameraid;
    hmin = 0;
    smin = 0;
    vmin = 0;
    hmax = 255;
    smax = 255;
    vmax = 255;
}


void VideoColorDetector::Load(){

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

void VideoColorDetector::Detect(cv::Mat& img, cv::Mat& hsv, cv::Mat& mask){

    if (!cap.isOpened()) {
        std::cerr << "Camera not opened!" << std::endl;
        return;
    }

    cap.read(img);
    cvtColor(img, hsv, cv::COLOR_BGR2HSV);

    cv::Scalar lower(hmin, smin, vmin);
    cv::Scalar upper(hmax, smax, vmax);

    inRange(hsv, lower, upper, mask);
    /*
    std::cout << "Hue : " << hmin << " / " << hmax << std::endl;
    std::cout << "Sat : " << smin << " / " << smax << std::endl;
    std::cout << "Val : " << vmin << " / " << vmax << std::endl;
    */

}