#include "MorphologicalCapability.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

void MorphologicalCapability::execute(const cv::Mat& image) {
    MorphologicalProcessor processor;
    MorphologicalResult result = processor.process(image);
    DisplayManager display;
    cv::Size sz(640, 480);

    if (CapabilityContext::mode == "--side") {
        display.show("Original", image, sz, cv::Point(0, 0));
        display.show("Blurred", result.blurred, sz, cv::Point(640, 0));
        display.show("Edges", result.edges, sz, cv::Point(0, 520));
        display.show("Dilated", result.dilated, sz, cv::Point(640, 520));
        display.show("Eroded", result.eroded, sz, cv::Point(1280, 0));
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
    }
}
