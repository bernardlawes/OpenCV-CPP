#pragma once
#include "Capability.hpp"
#include "Annotator.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class AnnotatorCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{
        Annotator processor;
        cv::Mat annootated = processor.DrawAnnotations(image);
        DisplayManager display;
    
        cv::Size displayDims(480, 300);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            display.show("Annotated", annootated, displayDims, cv::Point(displayDims.width, 0));
            display.wait();
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Annotated", annootated, displayDims, std::nullopt);
            cv::waitKey(0);
        }
    }
};