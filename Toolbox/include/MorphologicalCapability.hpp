#pragma once
#include "Capability.hpp"
#include "MorphologicalProcessor.hpp"

class MorphologicalCapability : public Capability {
public:
    void execute(const cv::Mat& image) override;
};
