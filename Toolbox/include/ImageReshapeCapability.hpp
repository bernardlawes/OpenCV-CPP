#pragma once
#include "Capability.hpp"
#include "ImageReshape.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"


class ImageReshapeCapability : public Capability {
    public:
        void execute(const cv::Mat& image) override{
            ImageReshape reshaper;
            ImageReshapeResult result;

            result.resized = reshaper.resizer(image);
            result.rescaled = reshaper.rescale(image);
            result.cropped = reshaper.cropper(image);

            DisplayManager display;
        
            
        
            if (CapabilityContext::mode == "--side") {
                cv::Size sz(480, 300);
                display.show("Original", image, sz, cv::Point(0, 0));       
                display.show("Resized", result.resized, sz, cv::Point(sz.width, 0));
                display.show("Rescaled", result.rescaled, sz, cv::Point(sz.width*2, 0));
                display.show("Cropped", result.cropped, sz, cv::Point(sz.width*3, 0));
                display.wait();
            } else {
                cv::Size sz(1920, 1200);
                display.show("Original", image, sz, std::nullopt);
                cv::waitKey(0);
                display.show("Resized", result.resized, sz, std::nullopt);
                cv::waitKey(0);
                display.show("Rescaled", result.rescaled, sz, std::nullopt);
                cv::waitKey(0);
                display.show("Cropped", result.cropped, sz, std::nullopt);
                
                cv::waitKey(0);
            }
        }
    };