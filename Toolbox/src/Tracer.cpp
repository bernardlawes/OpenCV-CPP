#include "Tracer.hpp"
#include <iostream>

TraceResult Tracer::Trace(const cv::Mat& img){
    
    TraceResult result;
    cv::Mat cloned = img.clone();
    cv::Mat greyed;
    

    // findContours(), but it only supports single-channel grayscale (CV_8UC1) images, unless you're using RETR_FLOODFILL or RETR_CCOMP
    cvtColor(cloned, greyed, cv::COLOR_BGR2GRAY);  // Convert to grayscale

    // Calculate mean brightness
    double meanBrightness = cv::mean(greyed)[0];
    result.meanBrightness = static_cast<int>(meanBrightness);

    // Invert if background is likely white
    if (meanBrightness > 127) {
        cv::bitwise_not(greyed, greyed);  // invert to make shapes white on black
    }

    cv::Mat binary;

    // Threshold or adaptive threshold
    cv::threshold(greyed, binary, 50, 255, cv::THRESH_BINARY);


    //cv::imshow("Gray", greyed);
    //cv::waitKey(0);
    //cv::imshow("Binary", binary);
    //cv::waitKey(0);


    std::vector<std::vector<cv::Point>>contours; // contours are vector of vectors of points
	std::vector<cv::Vec4i> heirarchy; // 4 element vector of integers

    // find the contours
	findContours(binary, contours, heirarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);  // Retrieves only outermost contours
    //findContours(cloned, contours, heirarchy, cv::RETR_LIST, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours without hierarchy
    //findContours(cloned, contours, heirarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);  // Retrieves all contours with full hierarchy
    //findContours(cloned, contours, heirarchy, cv::RETR_CCOMP, cv::CHAIN_APPROX_SIMPLE); // Retrieves all contours in a 2-level hierarchy (parent/child)


    // get contour count
    int ccount = contours.size();

	// draw all the contours
	//drawContours(cloned, contours, -1, cv::Scalar(0, 0, 255), 2); // -1 to draw all contours

    
    // container for approximated (simplified) contours | a 2D vector (a vector of vectors) 
    std::vector<std::vector<cv::Point>> approxCon(ccount); // the number of points will be less than or equal to the number of points in the original contour

    std::cout << "Contour Count: " << ccount << std::endl;

    result.contours         = contours;
    result.heirarchy        = heirarchy;
    result.count            = ccount;
    result.image            = cloned;
    //result.filtered       = img.clone();
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

    float sensitivity = 0.01; // used in approximation of shape; the lower the value, the more sensitive the funcition
    bool  simple = true;
    std::vector<std::vector<cv::Point>> approxContours(input.count);
    //cv::Scalar ccolor = cv::Scalar(0,255,255);
    //cv::Scalar tcolor = cv::Scalar(255,255,255);
    cv::Mat greyed;

        // Calculate mean brightness
        cvtColor(input.image, greyed, cv::COLOR_BGR2GRAY);  // Convert to grayscale
        //double meanBrightness = cv::mean(greyed)[0];
        //input.meanBrightness = static_cast<int>(meanBrightness);

        // Invert if background is likely white
        //if (meanBrightness > 127) {
        //    tcolor = cv::Scalar(0,0,0);
        //}

    std::cout << "Thresholds: " << thresholds[0] << thresholds[1] << std::endl;

    for (int i=0; i < input.count; i++){

        // reset properties on each iteration
        float                               aspect=0;
        int                                 area;
        int                                 perimeter;
        int                                 segments;
        std::string                         shapename = "undefined";
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
            shapename       = GetShape(approxContours[i]);

            input.filteredContourIndices.push_back(i); // add index of the contour to the filtered list
            input.filteredContours.push_back(contour); // add contour to the filtered list

            // update output
            input.approxContours[i] = approxContours[i];
            input.areas[i]          = area;
            input.perimeters[i]     = perimeter;
            input.aspects[i]        = aspect;
            input.bbox[i]           = bbox;
            input.segments[i]       = segments;
            input.midpoints[i]      = centroid;
            input.shapes[i]         = shapename;

            // either draw simple or actual contour (option based) of the specified contour by i
            //(simple) ? cv::drawContours(input.filtered, input.approxContours, i, ccolor,3 ) : cv::drawContours(input.filtered, input.contours, i, ccolor,3 );

            // write shape name over the shape
            //putText(input.filtered, shapename, cv::Point(centroid.x-50,centroid.y), cv::FONT_HERSHEY_DUPLEX, 0.85, tcolor, 1);

        }

    }

    // either draw simple or actual contour (option based) of the specified contour by i [This version draws all at once]
    //(simple) ? cv::drawContours(input.curated, input.approxContours, -1, ccolor,3 ) : cv::drawContours(input.curated, input.contours, -1, ccolor,3 );

}

std::string Tracer::Inspect_Quadrilateral(const std::vector<cv::Point>& approxContour){

        // Get side vectors
        std::vector<double> slopes;
        for (int i = 0; i < 4; ++i) {
            cv::Point p1 = approxContour[i];
            cv::Point p2 = approxContour[(i + 1) % 4];
            double dx = p2.x - p1.x;
            double dy = p2.y - p1.y;
            slopes.push_back(dx != 0 ? dy / dx : 1e9);  // avoid division by zero
        }

        // Count how many pairs of sides have similar slopes (indicating parallel lines)
        int parallelCount = 0;
        for (int i = 0; i < 2; ++i) {
            double s1 = slopes[i];
            double s2 = slopes[i + 2];
            if (std::abs(s1 - s2) < 0.1) // slope tolerance
                parallelCount++;
        }

        // Bounding rectangle aspect ratio
        cv::Rect boundingRect = cv::boundingRect(approxContour);
        float aspectRatio = static_cast<float>(boundingRect.width) / boundingRect.height;

        if (parallelCount == 2) {
            if (aspectRatio >= 0.95 && aspectRatio <= 1.05)
                return "Square";
            else
                return "Rectangle";
        } else if (parallelCount == 1) {
            return "Trapezoid";
        } else {
            return "Irregular Quad";
        }
}   

std::string Tracer::GetShape(const std::vector<cv::Point>& approxContour){

    std::string shape = "";

    cv::Rect boundingRect = cv::boundingRect(approxContour); // Compute bounding rectangle
    float aspectRatio = static_cast<float>(boundingRect.width) / boundingRect.height;


    switch (approxContour.size()) {
        // Check if it's a triangle
        case 3:
            shape = "Triangle";
            break;
        // Check if it's a square or rectangle by computing the aspect ratio
        case 4:
            shape = Inspect_Quadrilateral(approxContour);
            break;
        case 5:
            shape = "Pentagon";
            break; 
        default:
            shape = "Round";
            // Compute circularity = 4π * Area / (Perimeter^2)
            double area = cv::contourArea(approxContour);
            double perimeter = cv::arcLength(approxContour, true);
            double circularity = 4 * CV_PI * area / (perimeter * perimeter);
            if (circularity > 0.6) {
                shape =  "Round";
                switch (approxContour.size()) {
                    case 6:
                        shape = "Hexagon";
                        break;
                    case 7: 
                        shape = "Heptagon";
                        break;  
                    case 8:
                        shape = "Octagon";
                        break;  
                    case 9:
                        shape = "Nonagon";
                        break;
                    case 10:
                        shape = "Decagon";
                        break;
                    default:
                        if (aspectRatio >= 0.95 && aspectRatio <= 1.05 && circularity > 0.8) {
                            shape = "Circle"; // If aspect ratio is close to 1, it's a circle
                        } else {
                            shape = "Eclipse"; // Otherwise, it's an elipse
                        }
                        break;
                }
            } else {
                shape = "Ambiguous"; // If circularity is low, we can't determine the shape
            }
            break;
    }

    return shape;
}

cv::Point Tracer::GetCentroid(const cv::Rect& bbox){
    int cx = bbox.x + bbox.width/2;
    int cy = bbox.y + bbox.height/2;
    return cv::Point(cx,cy);
}