#include "ImageLoader.hpp"
#include "ColorDetector.hpp"
#include "DisplayManager.hpp"
#include "Orchestrator.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    

    if (argc < 2) {
        std::cerr << "\n[DEBUG] No image path provided." << std::endl << std::flush;
        return 1;
    }

    if (argc < 3) {
        std::cerr << "\n[DEBUG] Usage: ./CVSandbox <image_path> <ability_name>" << std::endl << std::flush;
        return 1;
    }

    std::string imagePath = argv[1];
    std::string ability = argv[2];

    ImageLoader loader;
    cv::Mat image = loader.load(imagePath);
    if (image.empty()) {
        std::cerr << "Failed to load image: " << imagePath << std::endl;
        return 1;
    }

    Orchestrator orchestrator;
    orchestrator.run(ability, image);

    return 0;
}