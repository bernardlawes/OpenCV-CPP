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
    
        cv::Size sz(480, 300);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, sz, cv::Point(0, 0));
            display.show("Blurred", result.blurred, sz, cv::Point(sz.width, 0));
    
            display.show("Edges", result.edges, sz, cv::Point(0, sz.height + 40));
            display.show("Dilated", result.dilated, sz, cv::Point(sz.width, sz.height + 40));
    
            display.show("Eroded", result.eroded, sz, cv::Point(sz.width*2, 0));
            display.show("Grayed", result.grayed, sz, cv::Point(sz.width*2, sz.height + 40));
            display.wait();
        } else {
            display.show("Original", image, sz, std::nullopt);
            cv::waitKey(0);
            display.show("Blurred", result.blurred, sz, std::nullopt);
            cv::waitKey(0);
            display.show("Edges", result.edges, sz, std::nullopt);
            cv::waitKey(0);
            display.show("Dilated", result.dilated, sz, std::nullopt);
            cv::waitKey(0);
            display.show("Eroded", result.eroded, sz, std::nullopt);
            cv::waitKey(0);
            display.show("Grayed", result.grayed, sz, std::nullopt);
            cv::waitKey(0);
        }
    }
};
