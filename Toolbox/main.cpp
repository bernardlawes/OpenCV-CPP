#include "ImageLoader.hpp"
#include "ColorDetector.hpp"
#include "DisplayManager.hpp"
#include "Orchestrator.hpp"
#include <iostream>
#include <string>

int main(int argc, char* argv[]) {
    Orchestrator orchestrator;

    if (argc < 3 || std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help") {
        std::cout << "Usage: ./CVSandbox <image_path> <capability> [--step|--side]\n\n";
        std::cout << "Available capabilities:\n";
        for (const auto& name : orchestrator.listCapabilities()) {
            std::cout << "  - " << name << "\n";
        }
        return 0;
    }

    std::string imagePath = argv[1];
    std::string capability = argv[2];
    std::string mode = (argc >= 4) ? argv[3] : "";

    ImageLoader loader;
    cv::Mat image = loader.load(imagePath);
    if (image.empty()) {
        std::cerr << "[DEBUG] Failed to load image: " << imagePath << std::endl;
        return 1;
    }

    orchestrator.run(capability, image, mode);
    return 0;
}
