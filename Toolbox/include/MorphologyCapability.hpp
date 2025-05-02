#pragma once
#include "Capability.hpp"
#include "Morphology.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class MorphologyCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{
        MorphologyProcessor processor;
        MorphologyResult result = processor.process(image);
        DisplayManager display;
    
        cv::Size displayDims(480, 300);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            display.show("Blurred", result.blurred, displayDims, cv::Point(displayDims.width, 0));
    
            display.show("Edges", result.edges, displayDims, cv::Point(0, displayDims.height + 40));
            display.show("Dilated", result.dilated, displayDims, cv::Point(displayDims.width, displayDims.height + 40));
    
            display.show("Eroded", result.eroded, displayDims, cv::Point(displayDims.width*2, 0));
            display.show("Grayed", result.grayed, displayDims, cv::Point(displayDims.width*2, displayDims.height + 40));
            display.wait();
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Blurred", result.blurred, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Edges", result.edges, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Dilated", result.dilated, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Eroded", result.eroded, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Grayed", result.grayed, displayDims, std::nullopt);
            cv::waitKey(0);
        }
    }
};
