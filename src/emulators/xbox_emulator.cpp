#include "xbox_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

namespace XEmuRun {

XboxEmulator::XboxEmulator(const std::string& xboxVersion)
    : BaseEmulator("Xbox Emulator", xboxVersion), m_xboxVersion(xboxVersion) {
}

XboxEmulator::~XboxEmulator() = default;

bool XboxEmulator::initialize() {
    if (!BaseEmulator::initialize()) {
        return false;
    }
    
    return setupEmulator();
}

bool XboxEmulator::setupEmulator() {
    // Determine which emulator to use based on platform
    if (m_xboxVersion == "xbox") {
        m_emulatorBinary = "xemu"; // Xbox emulator
        
        // Check if the emulator is installed
        int result = std::system("which xemu > /dev/null 2>&1");
        if (result != 0) {
            std::cerr << "Xbox emulator (xemu) is not installed." << std::endl;
            return false;
        }
    } 
    else if (m_xboxVersion == "xbox_series") {
        m_emulatorBinary = "xenia"; // Xbox Series emulator
        
        // Check if the emulator is installed
        int result = std::system("which xenia > /dev/null 2>&1");
        if (result != 0) {
            std::cerr << "Xbox Series emulator (xenia) is not installed." << std::endl;
            return false;
        }
    }
    else {
        std::cerr << "Unsupported Xbox version: " << m_xboxVersion << std::endl;
        return false;
    }
    
    std::cout << "Xbox emulator configured: " << m_emulatorBinary << std::endl;
    return true;
}

int XboxEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize Xbox emulator" << std::endl;
        return 1;
    }
    
    std::string gamePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();
    
    if (!fs::exists(gamePath)) {
        std::cerr << "Game file not found: " << gamePath << std::endl;
        return 1;
    }
    
    // Prepare command with emulator
    std::string command = m_emulatorBinary + " \"" + gamePath + "\"";
    
    std::cout << "Launching Xbox game: " << gamePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    // Execute the command
    return std::system(command.c_str());
}

} // namespace XEmuRun
