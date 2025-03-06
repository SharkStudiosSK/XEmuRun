#include <iostream>
#include <string>
#include "launcher/launcher.h"
#include "config/config_manager.h"

int main(int argc, char* argv[]) {
    std::cout << "XEmuRun - Universal Game Emulation Platform" << std::endl;
    
    // Initialize configuration system
    XEmuRun::ConfigManager& configManager = XEmuRun::ConfigManager::getInstance();
    if (!configManager.initialize()) {
        std::cerr << "Failed to initialize configuration system" << std::endl;
        return 1;
    }
    
    if (argc < 2) {
        std::cout << "Usage: xemurun [path_to_xemupkg]" << std::endl;
        return 1;
    }

    std::string packagePath = argv[1];
    XEmuRun::Launcher launcher;
    
    if (launcher.loadPackage(packagePath)) {
        return launcher.runGame();
    } else {
        std::cerr << "Failed to load package: " << packagePath << std::endl;
        return 1;
    }
}
