#pragma once
#include "Capability.hpp"
#include "WarpPerspective.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class WarpPerspectiveCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{
        WarpPerspective processor;
        cv::Mat annotated = processor.Annotate(image);
        cv::Mat warped = processor.Orthogonize(image);
        DisplayManager display;
    
        cv::Size displayDims(600, 400);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            display.show("Annotated", annotated, displayDims, cv::Point(displayDims.width, 0));
            display.show("Warped", warped, cv::Size(250,350), cv::Point(displayDims.width*2, 0));
            display.wait();
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Annotated", annotated, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Warped", warped, cv::Size(250,350), std::nullopt);
            cv::waitKey(0);
        }
    }
};