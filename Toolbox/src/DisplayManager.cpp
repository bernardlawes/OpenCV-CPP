
#include "DisplayManager.hpp"

// C++17 or newer, std::optional avoids the need for pointers and keeps the code clean:
// std::optional is used to handle optional parameters for dimensions and location.
// This allows the user to specify a size and location for the window, or use default values if not provided.
// The DisplayManager class is responsible for displaying images in a window.
// When using std::optional, one must first check if the value exists (i.e. has been provided), and then access its members using .value()

void DisplayManager::show(const std::string& windowName, const cv::Mat& image, std::optional<cv::Size> dimensions = std::nullopt, std::optional<cv::Point> location = std::nullopt) {

    // Check if the image is empty
    if (image.empty()) {
        std::cerr << "[DEBUG] Image is empty, cannot display." << std::endl << std::flush;
        return;
    }

    cv::namedWindow(windowName, cv::WINDOW_NORMAL); 

    // Resize if dimensions are provided
    if (dimensions) {
        cv::resizeWindow(windowName, dimensions->width, dimensions->height);
    } else {
        cv::resizeWindow(windowName, 640, 480); // default fallback
    }

    // Move if location is provided
    if (location) {
        cv::moveWindow(windowName, location->x, location->y);
    } else {
        cv::moveWindow(windowName, 0, 0); // default fallback
    }

    cv::imshow(windowName, image);
    
    
}

void DisplayManager::wait(int delayMs) {
    cv::waitKey(delayMs);
}