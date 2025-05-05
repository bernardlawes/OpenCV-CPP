#pragma once
#include "Capability.hpp"
#include "FaceDetector.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class FaceDetectorCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{

        std::string haarFilename = "input/haarcascade_frontalface_default.xml";
        cv::Mat photo;

        FaceDetector processor;
        if (!processor.LoadHaarCascade(haarFilename)){
            std::cout << "Failed to Load Haar Cascade: "+haarFilename;
            return;
        } else {
            std::cout << "Loaded Haar Cascade: "+haarFilename;
        }

        int facecount = processor.DetectFaces(image);
        if ( facecount < 1){
            std::cout << "No Faces Found: ";
            return;
        } else {
            std::cout <<  facecount << " Faces Found" << std::endl;
            processor.DrawBBOX();
        }



        DisplayManager display;
    
        cv::Size displayDims(800, 600);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            display.show("Markup", processor.markup, displayDims, cv::Point(displayDims.width, 0));
            std::string windowName = "Cropped Face";
            //cv::namedWindow(windowName, cv::WINDOW_NORMAL); 
            //cv::moveWindow(windowName, displayDims.width*2, 0);
            for (int i = 0; i < processor.faces.size(); ++i) {
                cv::Mat face = processor.CropFace(i);
                if (!face.empty()) {
                    //cv::imshow(windowName, face);
                    display.show("Cropped Face", face, displayDims, cv::Point(displayDims.width*2, 0));
                    cv::waitKey(500);
                }
            }
            //display.show("Curated", result.curated, displayDims, cv::Point(displayDims.width*2, 0));
            //display.wait();
            cv::waitKey(0);
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            //display.show("Markup", result.markup, displayDims, std::nullopt);
            cv::waitKey(0);
            //display.show("Curated", result.curated, displayDims, std::nullopt);
            cv::waitKey(0);
        }
    }
};