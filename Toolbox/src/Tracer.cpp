#include "Tracer.hpp"
#include <iostream>

TraceResult Tracer::Trace(const cv::Mat& img){
    
    TraceResult result;
    cv::Mat markup = img.clone();
    cv::Mat greyed;

    // findContours(), but it only supports single-channel grayscale (CV_8UC1) images, unless you're using RETR_FLOODFILL or RETR_CCOMP
    cvtColor(markup, greyed, cv::COLOR_BGR2GRAY);  // Convert to grayscale

    std::vector<std::vector<cv::Point>>contours; // contours are vector of vectors of points
	std::vector<cv::Vec4i> heirarchy; // 4 element vector of integers

    // find the contours
	findContours(greyed, contours, heirarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  // Retrieves only outermost contours
    //findContours(markup, contours, heirarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours without hierarchy
    //findContours(markup, contours, heirarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours with full hierarchy
    //findContours(markup, contours, heirarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE); // Retrieves all contours in a 2-level hierarchy (parent/child)


    // get contour count
    int ccount = contours.size();

	// draw all the contours
	drawContours(markup, contours, -1, cv::Scalar(0, 0, 255), 2); // -1 to draw all contours

    
    // approximated contours | a 2D vector (a vector of vectors) 
    std::vector<std::vector<cv::Point>> approxCon(ccount); // the number of points will be less than or equal to the number of points in the original contour

    std::cout << "Contour Count: " << ccount << std::endl;

    result.contours         = contours;
    result.heirarchy        = heirarchy;
    result.count            = ccount;
    result.markup           = markup;
    result.curated          = img.clone();
    result.approxContours   = std::vector<std::vector<cv::Point>>(ccount);
    result.bbox             = std::vector<cv::Rect>(ccount);
    result.areas            = std::vector<int>(ccount);
    result.perimeters       = std::vector<int>(ccount);
    result.segments         = std::vector<int>(ccount);
    result.midpoints        = std::vector<cv::Point>(ccount);
    result.aspects          = std::vector<float>(ccount);
    result.shapes           = std::vector<std::string>(ccount);

    return result;
}

void Tracer::Screen(TraceResult& input, const std::vector<int>& thresholds, const bool& categorize){

    float sensitivity = 0.02; // used in approximation of shape; the lower the value, the more sensitive the funcition
    bool  simple = true;
    std::vector<std::vector<cv::Point>> approxCon(input.count);

    std::cout << "Thresholds: " << thresholds[0] << thresholds[1] << std::endl;

    for (int i=0; i < input.count; i++){

        // reset properties on each iteration
        float                               aspect=0;
        int                                 area;
        int                                 perimeter;
        int                                 segments;
        std::string                         shape = "undefined";
        cv::Point                           centroid;
        std::vector<cv::Point>              contour;
        cv::Scalar                          contourColor = cv::Scalar(0,255,255);
        //std::vector<std::vector<cv::Point>> approxContours;
        std::vector<cv::Point>              approxCon;
        cv::Rect                            bbox;


        // get single contour
        contour = input.contours[i];

        // calculate area
        area = cv::contourArea(contour);

        std::cout << "Area: " << area << std::endl;

        if (area > thresholds[0] && area < thresholds[1]){
            
            std::cout << "Processing: " << i << std::endl;

            // get perimeter
            perimeter = cv::arcLength(contour, true);

            
            // get approximate contour; useful for determining shape; the smaller the multiplication factor, the more sensitve it is to finding points
            cv::approxPolyDP(contour, approxCon, sensitivity * perimeter, true);
            segments = approxCon.size();

            input.approxContours[i] = approxCon;
            
            // draw simple or actual contour based on option of the contour specified by i
            if (simple){
                cv::drawContours(input.curated, input.approxContours, i, cv::Scalar(0,255,255),3 );
                std::cout << "Drew Contour: " << i << std::endl;
            } else {
                cv::drawContours(input.curated, input.contours, i, contourColor,3 );
            }
            
            // determine bounding box
            bbox = cv::boundingRect(approxCon);
            centroid = GetCentroid(bbox);
            shape = (categorize) ? Categorize(approxCon) : "";

            // update output
            input.areas[i] = area;
            input.perimeters[i] = perimeter;
            input.aspects[i] = aspect;
            input.approxContours[i] = approxCon;
            input.bbox[i] = bbox;
            input.segments[i] = segments;
            input.midpoints[i] = centroid;
            input.shapes[i] = shape;

            if (categorize){
                putText(input.curated, shape, centroid, cv::FONT_HERSHEY_DUPLEX, 1.0, cv::Scalar(255, 255, 255), 2);
            }

        }

    }

}

std::string Tracer::Categorize(const std::vector<cv::Point>& approxContour){

    std::string shape = "";

    switch (approxContour.size()) {
        case 3:
            shape = "TRI";
            break;
        case 4:
            shape = "TRAP";
            break;
        default:
            shape = "Round";
            break;
    }

    return shape;
}

cv::Point Tracer::GetCentroid(const cv::Rect& bbox){
    int cx = bbox.x + bbox.width/2;
    int cy = bbox.y + bbox.height/2;
    return cv::Point(cx,cy);
}