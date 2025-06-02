#pragma once
#include "Capability.hpp"
#include "Scanner.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class ScannerCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{

        Scanner processor(2);
        processor.Load();

        cv::Mat imgDilated, imgAnnotated;
        cv::Mat imgOrdered, imgWarped, imgCropped;


        if (!processor.cap.isOpened()) {
            std::cerr << "Error: Camera not opened." << std::endl;
            return; // Exit if camera cannot be opened
        }
        processor.Process(image);

        cv::Mat canvas = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);

        cv::Mat img, hsv, mask;
        DisplayManager display;
        cv::Size displayDims(600, 400);
        cv::Size objectDims(300, 200);
        
        while (true) {

            img = processor.GetFrame();
            if (img.empty()) {
                std::cerr << "Error: Frame is empty." << std::endl;
                break; // Exit if no frame is captured
            }

            imgDilated = processor.Process(img);
            std::vector<cv::Point> contour = processor.Trace(imgDilated);
            imgAnnotated = processor.Annotate(img, contour);

            // reorder points
            std::vector<cv::Point> orderedPoints = processor.Reorder(contour);

            if (orderedPoints.size() < 4) {
                //std::cerr << "[ERROR] Points not ordered." << std::endl;
                //continue; // Skip to the next iteration if not enough points
                imgOrdered = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
                imgWarped = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
                imgCropped = cv::Mat::zeros(objectDims, CV_8UC3);
            } else {
                // annotate the newly ordered points
                imgOrdered = processor.Annotate(img, orderedPoints);

                // warp the image using the ordered points
                imgWarped = processor.Warp(img, orderedPoints, objectDims.width, objectDims.height);

                // crop the warped image to the region of interest
                imgCropped = processor.Crop(imgWarped, objectDims.width, objectDims.height);
            }


            

            display.show("Original", img, displayDims, cv::Point(0, 0));
            display.show("Processed", imgDilated, displayDims, cv::Point(displayDims.width, 0));
            display.show("Annotated", imgAnnotated, displayDims, cv::Point(displayDims.width*2, 0));

            display.show("Ordered", imgOrdered, displayDims, cv::Point(0, displayDims.height));
            display.show("Warped", imgWarped, displayDims, cv::Point(displayDims.width, displayDims.height));
            display.show("Cropped", imgCropped, displayDims, cv::Point(displayDims.width*2, displayDims.height));

            // Wait for a key press for 1ms
            char key = (char)cv::waitKey(1);
            if (key == 27) break;                // ESC = exit
            if (key == 'c') canvas = cv::Mat::zeros(canvas.size(), canvas.type()); // Clear canvas

        }


    }

};