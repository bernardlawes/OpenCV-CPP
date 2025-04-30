// Orchestrator.cpp
#include "Orchestrator.hpp"
#include "ColorDetectorCapability.hpp"

Orchestrator::Orchestrator() {
    capabilities["color"] = std::make_shared<ColorDetectorCapability>();
    // Add more capabilities here as you grow
}

void Orchestrator::run(const std::string& abilityName, const cv::Mat& image) {
    auto it = capabilities.find(abilityName);
    if (it != capabilities.end()) {
        it->second->execute(image);
    } else {
        std::cerr << "[DEBUG] Unknown capability: " << abilityName << std::endl;
    }
}
