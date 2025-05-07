#pragma once
#include <fstream>
#include "Capability.hpp"
#include "VideoColorTracker.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class VideoColorTrackerCapability : public Capability {
public:

    // variables used 

    cv::Point last_center{-1, -1}; // Last tracked point (used to draw lines)  // Could have also written: cv::Point last_center = cv::Point(-1, -1); // cv::Point last_center(-1, -1); would fail because it tries to initialize a member variable inline using constructor-like syntax
    cv::Size displayDims = cv::Size(640, 480);
    cv::Mat frame, hsv, mask, overlay;

    // Create a blank image (black canvas) to draw on
    cv::Mat canvas = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3);
    bool paint_enabled = true;

    void execute(const cv::Mat& image) override{

        DisplayManager display;

        VideoColorTracker processor(0);
        processor.Load();

        while (true) {
            processor.Detect(frame, hsv, mask);

            // Apply morphological operations to clean up the mask
            processor.Clean(mask);

            // Find contours in the binary mask, filter the contours, computer centroid, update the canvas, 
            processor.Trace(mask, canvas, last_center, paint_enabled);

            //annotate the frame with the detected blob
            processor.Marker(frame,last_center);

            // Overlay the canvas onto the video feed
            processor.Overlay(frame, canvas, overlay);


            display.show("Original", frame, displayDims, cv::Point(0, 0));
            display.show("HSV", hsv, displayDims, cv::Point(displayDims.width, 0));
            display.show("Mask", mask, displayDims, cv::Point(displayDims.width*2, 0));
            display.show("Combined", overlay, displayDims, cv::Point(displayDims.width*2, displayDims.height));
            //display.wait(1);
            // Wait for a key press for 1ms
            char key = (char)cv::waitKey(1);
            if (key == 27) break;                // ESC = exit
            if (key == 'c') canvas = cv::Mat::zeros(canvas.size(), canvas.type()); // Clear canvas
            if (key == 'p') paint_enabled = !paint_enabled; // Toggle painting

        }
            
    }


};