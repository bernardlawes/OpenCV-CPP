#pragma once
#include "Capability.hpp"
#include "Tracer.hpp"
#include "DisplayManager.hpp"
#include "CapabilityContext.hpp"

class TracerCapability : public Capability {
public:
    void execute(const cv::Mat& image) override{


        std::vector<int> thresholds{3000, 30000};
        cv::Scalar textcolor = cv::Scalar(255,255,255);

        Tracer processor;
        TraceResult result = processor.Trace(image);
        processor.Filter(result, thresholds);

        if (result.meanBrightness > 127) {
            textcolor = cv::Scalar(0,0,255);
        }

        DisplayManager display;
    
        cv::Size displayDims(800, 600);
    
        if (CapabilityContext::mode == "--side") {
            display.show("Original", image, displayDims, cv::Point(0, 0));
            cv::waitKey(0);
            
            // draw all the contours
            cv::Mat markup = image.clone(); //cv::Mat::zeros(image.size(), image.type());
	        drawContours(markup, result.contours, -1, cv::Scalar(0, 0, 255), 2); // -1 to draw all contours
            display.show("Markup All", markup, displayDims, cv::Point(displayDims.width, 0));
            cv::waitKey(0);

            // draw filtered contours
            markup = image.clone(); // reset markup
            drawContours(markup, result.filteredContours, -1, cv::Scalar(0, 255, 0), 2); // -1 to draw all contours
            display.show("Markup Filtered by Size (Direct)", markup, displayDims, cv::Point(displayDims.width*2, 0));
            cv::waitKey(0);

            // draw filtered contours on the original image using indices
            markup = image.clone(); // reset markup
            for (int idx : result.filteredContourIndices) {
                cv::drawContours(markup, result.contours, idx, cv::Scalar(0, 255, 0), 2);
            }
            display.show("Markup Filtered by Size (Indices)", markup, displayDims, cv::Point(0, displayDims.height));
            cv::waitKey(0);




            // draw size-filtered approximated contours, and their classfied shape names
            markup = image.clone(); // reset markup
            drawContours(markup, result.approxContours, -1, cv::Scalar(0, 255, 0), 2); // -1 to draw all contours
            for (size_t i = 0; i < result.approxContours.size(); ++i) {
                cv::putText(markup, result.shapes[i], cv::Point(result.midpoints[i].x-50, result.midpoints[i].y), cv::FONT_HERSHEY_SIMPLEX, 0.8, textcolor, 2);
            }
            display.show("Markup Filtered by Size (Direct) with Shape Names", markup, displayDims, cv::Point(displayDims.width, displayDims.height));
            cv::waitKey(0);



            // draw curated contours - filtered by shape name
            markup = image.clone(); // reset markup
            //drawContours(markup, result.approxContours, -1, cv::Scalar(0, 255, 0), 3); // -1 to draw all contours
            std::vector<std::string> shapeNames = {"Circle", "Eclipse", "Rectangle", "Square", "Triangle","Trapezoid", "Pentagon", "Hexagon", "Heptagon", "Octagon", "Nonagon", "Decagon"};
            for (size_t i = 0; i < result.approxContours.size(); ++i) {
                if (std::find(shapeNames.begin(), shapeNames.end(), result.shapes[i]) != shapeNames.end()) {
                    cv::drawContours(markup, result.approxContours, i, cv::Scalar(0, 255, 0), 2);
                    cv::putText(markup, result.shapes[i], cv::Point(result.midpoints[i].x-50, result.midpoints[i].y), cv::FONT_HERSHEY_SIMPLEX, 0.8, textcolor, 2);
                }
            }
            display.show("Markup Filtered by Shape", markup, displayDims, cv::Point(displayDims.width*2,  displayDims.height));
            cv::waitKey(0);            

            display.wait();
        } else {
            display.show("Original", image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Markup", result.image, displayDims, std::nullopt);
            cv::waitKey(0);
            display.show("Curated", result.image, displayDims, std::nullopt);
            cv::waitKey(0);
        }
    }
};