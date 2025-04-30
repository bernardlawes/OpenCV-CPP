// Orchestrator.hpp
#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include "Capability.hpp"

class Orchestrator {
public:
    Orchestrator();
    void run(const std::string& abilityName, const cv::Mat& image);

private:
    std::unordered_map<std::string, std::shared_ptr<Capability>> capabilities;
};
