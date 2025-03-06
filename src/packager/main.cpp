#include <iostream>
#include <string>
#include <filesystem>
#include "packager.h"
#include "../config/config_manager.h"

namespace fs = std::filesystem;

void printUsage() {
    std::cout << "XEmuPackager - Create game packages for XEmuRun\n\n";
    std::cout << "Usage: xemupackager --game-path <path> --output-path <path> \n";
    std::cout << "                    --name <game_name> --platform <platform> \n";
    std::cout << "                    --main <executable_path> [--config key=value ...]\n\n";
    std::cout << "Options:\n";
    std::cout << "  --game-path <path>      Path to the game files\n";
    std::cout << "  --output-path <path>    Where to save the .XEmupkg file\n";
    std::cout << "  --name <game_name>      Name of the game\n";
    std::cout << "  --platform <platform>   Platform (windows, linux, playstation4, etc.)\n";
    std::cout << "  --main <executable>     Main executable path (relative to game directory)\n";
    std::cout << "  --config key=value      Add configuration key-value pair\n";
    std::cout << "  --auto-detect           Try to automatically detect platform\n";
    std::cout << "  --gui                   Launch the graphical interface\n";
    std::cout << "  --help, -h              Display this help message\n";
}

bool autoDetectPlatform(const std::string& gamePath, std::string& platform, std::string& mainExecutable) {
    std::cout << "Attempting to auto-detect platform and executable...\n";
    
    // Check for Windows executables (.exe files)
    bool foundWindows = false;
    
    try {
        for (const auto& entry : fs::recursive_directory_iterator(gamePath)) {
            if (entry.is_regular_file() && entry.path().extension() == ".exe") {
                if (!foundWindows) {
                    // First .exe file found
                    foundWindows = true;
                    mainExecutable = fs::relative(entry.path(), fs::path(gamePath)).string();
                    platform = "windows";
                    std::cout << "Detected Windows executable: " << mainExecutable << std::endl;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error scanning directory: " << e.what() << std::endl;
        return false;
    }
    
    if (foundWindows) {
        return true;
    }
    
    // Check for Linux executables (files with execute permission but no extension)
    try {
        for (const auto& entry : fs::recursive_directory_iterator(gamePath)) {
            if (entry.is_regular_file() && entry.path().extension() == "") {
                fs::perms p = fs::status(entry.path()).permissions();
                if ((p & fs::perms::owner_exec) != fs::perms::none) {
                    mainExecutable = fs::relative(entry.path(), fs::path(gamePath)).string();
                    platform = "linux";
                    std::cout << "Detected Linux executable: " << mainExecutable << std::endl;
                    return true;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error checking file permissions: " << e.what() << std::endl;
        return false;
    }
    
    // No platform detected
    std::cerr << "Could not auto-detect platform. Please specify manually." << std::endl;
    return false;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }
    
    // Initialize configuration system
    XEmuRun::ConfigManager& configManager = XEmuRun::ConfigManager::getInstance();
    if (!configManager.initialize()) {
        std::cerr << "Failed to initialize configuration system" << std::endl;
        return 1;
    }
    
    // Check for GUI mode
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--gui") {
            std::cout << "Starting GUI mode..." << std::endl;
            // Code to launch GUI would go here
            std::cerr << "GUI mode not implemented yet. Use command line options." << std::endl;
            return 1;
        }
    }
    
    XEmuRun::Packager packager;
    bool autoDetect = false;
    
    // Parse command line arguments
    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        
        if (arg == "--game-path" && i + 1 < argc) {
            packager.setGamePath(argv[++i]);
        } else if (arg == "--output-path" && i + 1 < argc) {
            packager.setOutputPath(argv[++i]);
        } else if (arg == "--name" && i + 1 < argc) {
            packager.setGameName(argv[++i]);
        } else if (arg == "--platform" && i + 1 < argc) {
            packager.setPlatform(argv[++i]);
        } else if (arg == "--main" && i + 1 < argc) {
            packager.setMainExecutable(argv[++i]);
        } else if (arg == "--config" && i + 1 < argc) {
            std::string configArg = argv[++i];
            size_t pos = configArg.find('=');
            if (pos != std::string::npos) {
                std::string key = configArg.substr(0, pos);
                std::string value = configArg.substr(pos + 1);
                packager.addConfigValue(key, value);
            } else {
                std::cerr << "Invalid config format. Expected key=value, got: " << configArg << std::endl;
            }
        } else if (arg == "--auto-detect") {
            autoDetect = true;
        } else if (arg == "--help" || arg == "-h") {
            printUsage();
            return 0;
        } else if (arg == "--gui") {
            // Already handled above
        } else {
            std::cerr << "Unknown argument: " << arg << std::endl;
            printUsage();
            return 1;
        }
    }
    
    // Apply auto-detection if requested
    if (autoDetect) {
        std::string gamePath = packager.getGamePath();
        if (!gamePath.empty()) {
            std::string platform;
            std::string mainExecutable;
            if (autoDetectPlatform(gamePath, platform, mainExecutable)) {
                // Only set these if they weren't specified manually
                if (packager.getPlatform().empty()) {
                    packager.setPlatform(platform);
                }
                if (packager.getMainExecutable().empty()) {
                    packager.setMainExecutable(mainExecutable);
                }
            }
        } else {
            std::cerr << "Game path must be specified for auto-detection" << std::endl;
            return 1;
        }
    }
    
    // If output path is not set, use the default from config
    if (packager.getOutputPath().empty()) {
        std::string defaultOutputDir = configManager.getSystemConfig().getString(
            "default_output_directory", "");
        
        if (!defaultOutputDir.empty()) {
            packager.setOutputPath(defaultOutputDir);
        } else {
            // Default to current directory
            packager.setOutputPath(".");
        }
    }
    
    // Create the package
    if (packager.createPackage()) {
        std::cout << "Package created successfully!" << std::endl;
        return 0;
    } else {
        std::cerr << "Failed to create package" << std::endl;
        return 1;
    }
}
