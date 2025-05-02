// Orchestrator.hpp
#pragma once
#include <opencv2/opencv.hpp>
#include <string>
#include <memory>
#include <unordered_map>
#include "Capability.hpp"


/*
OpenCV tries to dynamically load plugins (DLLs) for:

UI backends: GTK, GTK3, GTK2, WIN32, etc.

Parallelization: TBB, OpenMP, ONETBB

It searches in your configured paths:

D:\Repositories\Library\OpenCV\CPP\Toolbox\install\bin\
It logs attempts like:

load opencv_highgui_gtk4110_64.dll => FAILED
...and eventually says:

plaintext
Copy
Edit
UI: using backend: WIN32 (priority=970)
That means: GTK wasn't found, but WIN32 (native Windows) was available, so it's using that.

This is not a real issue;

You can suppress it with:
#include <opencv2/core/utils/logger.hpp>
cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_SILENT);  OR
cv::utils::logging::setLogLevel(cv::utils::logging::LOG_LEVEL_WARNING);

*/
class Orchestrator {
public:
    
    
    Orchestrator();
    void run(const std::string& abilityName, const cv::Mat& image, const std::string& mode = "");

    std::vector<std::string> listCapabilities() const;

private:
    std::unordered_map<std::string, std::shared_ptr<Capability>> capabilities;
};
