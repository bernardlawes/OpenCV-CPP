// Orchestrator.cpp
#include "Orchestrator.hpp"
#include "ColorDetectorCapability.hpp"
#include "MorphologyCapability.hpp"
#include "ImageReshapeCapability.hpp"
#include "AnnotatorCapability.hpp"
#include "WarpPerspectiveCapability.hpp"
#include "TracerCapability.hpp"
#include "FaceDetectorCapability.hpp"
#include "VideoColorDetectorCapability.hpp"
#include "VideoColorTrackerCapability.hpp"
#include "ScannerCapability.hpp"
#include "CapabilityContext.hpp"


Orchestrator::Orchestrator() {
    capabilities["color"] = std::make_shared<ColorDetectorCapability>();
    capabilities["morph"] = std::make_shared<MorphologyCapability>();
    capabilities["reshape"] = std::make_shared<ImageReshapeCapability>();
    capabilities["annotate"] = std::make_shared<AnnotatorCapability>();
    capabilities["warp"] = std::make_shared<WarpPerspectiveCapability>();
    capabilities["trace"] = std::make_shared<TracerCapability>();
    capabilities["face"] = std::make_shared<FaceDetectorCapability>();
    capabilities["videocolor"] = std::make_shared<VideoColorDetectorCapability>();
    capabilities["videocolortracker"] = std::make_shared<VideoColorTrackerCapability>();
    capabilities["scanner"] = std::make_shared<ScannerCapability>();

    // Adding more capabilities here as usecases grow
}

void Orchestrator::run(const std::string& abilityName, const cv::Mat& image, const std::string& mode) {
    auto it = capabilities.find(abilityName);
    if (it != capabilities.end()) {
        // Optional: set a global or static variable to pass mode to capability
        CapabilityContext::mode = mode;
        it->second->execute(image);
    } else {
        std::cerr << "[DEBUG] Unknown capability: " << abilityName << std::endl;
    }
}


std::vector<std::string> Orchestrator::listCapabilities() const {
    std::vector<std::string> keys;
    for (const auto& pair : capabilities) {
        keys.push_back(pair.first);
    }
    return keys;
}