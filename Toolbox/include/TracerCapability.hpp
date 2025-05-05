#pragma once
#include "Capability.hpp"
#include "Tracer.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class TracerCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{

        std::vector<int> thresholds{3000, 30000};

        Tracer processor;
        TraceResult result = processor.Trace(image);
        processor.Filter(result, thresholds);

        DisplayManager display;
    
        cv::Size displayDims(800, 600);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            display.show("Markup", result.markup, displayDims, cv::Point(displayDims.width, 0));
            display.show("Curated", result.curated, displayDims, cv::Point(displayDims.width*2, 0));
            display.wait();
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Markup", result.markup, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Curated", result.curated, displayDims, std::nullopt);
            cv::waitKey(0);
        }
    }
};