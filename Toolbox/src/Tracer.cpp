#include "Tracer.hpp"
#include <iostream>

TraceResult Tracer::Trace(const cv::Mat& img){
    
    TraceResult result;
    cv::Mat markup = img.clone();
    cv::Mat greyed;
    

    // findContours(), but it only supports single-channel grayscale (CV_8UC1) images, unless you're using RETR_FLOODFILL or RETR_CCOMP
    cvtColor(markup, greyed, cv::COLOR_BGR2GRAY);  // Convert to grayscale

    // Calculate mean brightness
    double meanBrightness = cv::mean(greyed)[0];

    // Invert if background is likely white
    if (meanBrightness > 127) {
        cv::bitwise_not(greyed, greyed);  // invert to make shapes white on black
    }

    cv::Mat binary;

    // Threshold or adaptive threshold
    cv::threshold(greyed, binary, 50, 255, cv::THRESH_BINARY);


    cv::imshow("Gray", greyed);
    cv::waitKey(0);
    cv::imshow("Binary", binary);
    cv::waitKey(0);


    std::vector<std::vector<cv::Point>>contours; // contours are vector of vectors of points
	std::vector<cv::Vec4i> heirarchy; // 4 element vector of integers

    // find the contours
	findContours(binary, contours, heirarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  // Retrieves only outermost contours
    //findContours(markup, contours, heirarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours without hierarchy
    //findContours(markup, contours, heirarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours with full hierarchy
    //findContours(markup, contours, heirarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE); // Retrieves all contours in a 2-level hierarchy (parent/child)


    // get contour count
    int ccount = contours.size();

	// draw all the contours
	drawContours(markup, contours, -1, cv::Scalar(0, 0, 255), 2); // -1 to draw all contours

    
    // container for approximated (simplified) contours | a 2D vector (a vector of vectors) 
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

/// @brief 
/// 
/// @param input 
/// @param thresholds 

void Tracer::Filter(TraceResult& input, const std::vector<int>& thresholds){

    float sensitivity = 0.02; // used in approximation of shape; the lower the value, the more sensitive the funcition
    bool  simple = true;
    std::vector<std::vector<cv::Point>> approxContours(input.count);
    cv::Scalar ccolor = cv::Scalar(0,255,255);
    cv::Scalar tcolor = cv::Scalar(255,255,255);
    cv::Mat greyed;

        // Calculate mean brightness
        cvtColor(input.curated, greyed, cv::COLOR_BGR2GRAY);  // Convert to grayscale
        double meanBrightness = cv::mean(greyed)[0];

        // Invert if background is likely white
        if (meanBrightness > 127) {
            tcolor = cv::Scalar(0,0,0);
        }

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
        cv::Rect                            bbox;


        // get single contour
        contour = input.contours[i];

        // calculate area
        area = cv::contourArea(contour);

        if (area > thresholds[0] && area < thresholds[1]){

            // get perimeter
            perimeter = cv::arcLength(contour, true);

            
            // get approximate contour; useful for determining shape; the smaller the multiplication factor, the more sensitve it is to finding points
            cv::approxPolyDP(contour, approxContours[i], sensitivity * perimeter, true);

            segments    = approxContours[i].size();
            bbox        = cv::boundingRect(approxContours[i]);
            centroid    = GetCentroid(bbox);
            shape       = GetShape(approxContours[i]);

            // update output
            input.approxContours[i] = approxContours[i];
            input.areas[i]          = area;
            input.perimeters[i]     = perimeter;
            input.aspects[i]        = aspect;
            input.bbox[i]           = bbox;
            input.segments[i]       = segments;
            input.midpoints[i]      = centroid;
            input.shapes[i]         = shape;

            // either draw simple or actual contour (option based) of the specified contour by i
            (simple) ? cv::drawContours(input.curated, input.approxContours, i, ccolor,3 ) : cv::drawContours(input.curated, input.contours, i, ccolor,3 );

            // write shape name over the shape
            putText(input.curated, shape, cv::Point(centroid.x-50,centroid.y), cv::FONT_HERSHEY_DUPLEX, 0.5, tcolor, 1);

        }

    }

    // either draw simple or actual contour (option based) of the specified contour by i [This version draws all at once]
    //(simple) ? cv::drawContours(input.curated, input.approxContours, -1, ccolor,3 ) : cv::drawContours(input.curated, input.contours, -1, ccolor,3 );

}

std::string Tracer::GetShape(const std::vector<cv::Point>& approxContour){

    std::string shape = "";

    switch (approxContour.size()) {
        case 3:
            shape = "Triangle";
            break;
        case 4:
            shape = "Trapezoid";
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