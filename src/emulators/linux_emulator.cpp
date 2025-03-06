#include "linux_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

namespace XEmuRun {

LinuxEmulator::LinuxEmulator()
    : BaseEmulator("Native Linux", "linux") {
}

LinuxEmulator::~LinuxEmulator() = default;

bool LinuxEmulator::initialize() {
    if (!BaseEmulator::initialize()) {
        return false;
    }
    
    return setupEnvironment();
}

bool LinuxEmulator::setupEnvironment() {
    // For Linux games, we might just need to check if common libraries are available
    std::cout << "Setting up Linux execution environment..." << std::endl;
    return true;
}

int LinuxEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize Linux emulator" << std::endl;
        return 1;
    }
    
    std::string executablePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();
    
    if (!fs::exists(executablePath)) {
        std::cerr << "Executable not found: " << executablePath << std::endl;
        return 1;
    }
    
    // Make the file executable
    std::string chmodCmd = "chmod +x \"" + executablePath + "\"";
    std::system(chmodCmd.c_str());
    
    // Prepare command
    std::string command = "\"" + executablePath + "\"";
    
    std::cout << "Launching Linux application: " << executablePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    // Execute the command
    return std::system(command.c_str());
}

} // namespace XEmuRun
