#pragma once
#include <fstream>
#include "Capability.hpp"
#include "VideoColorDetector.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class VideoColorDetectorCapability : public Capability {
public:

    std::string filename = "trackbars.config";

    void execute(const cv::Mat& image) override{

        VideoColorDetector processor(0);
        processor.Load();

        cv::Mat img, hsv, mask;
        DisplayManager display;
        cv::Size displayDims(600, 400);
        
        while (true) {
            processor.Detect(img, hsv, mask);

            display.show("Original", img, displayDims, cv::Point(0, 0));
            display.show("HSV", hsv, displayDims, cv::Point(displayDims.width, 0));
            display.show("Mask", mask, displayDims, cv::Point(displayDims.width*2, 0));
            display.wait(1);

            char key = (char)cv::waitKey(1);
            if (key == 's' || key == 'S') {
                saveHSVToFile(processor.hmin, processor.hmax,processor.smin, processor.smax,processor.vmin,processor.vmax);
            }

        }


    }

    void saveHSVToFile(int hmin, int hmax, int smin, int smax, int vmin, int vmax) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Error: Could not open " << filename << " for writing." << std::endl;
            return;
        }
    
        file << "HueMin=" << hmin << std::endl;
        file << "HueMax=" << hmax << std::endl;
        file << "SatMin=" << smin << std::endl;
        file << "SatMax=" << smax << std::endl;
        file << "ValMin=" << vmin << std::endl;
        file << "ValMax=" << vmax << std::endl;
    
        std::cout << "[INFO] HSV values saved to " << filename << std::endl;
    }
};