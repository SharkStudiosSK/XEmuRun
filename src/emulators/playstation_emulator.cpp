#include "playstation_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

namespace XEmuRun {

PlayStationEmulator::PlayStationEmulator(const std::string& playstationVersion)
    : BaseEmulator("PlayStation Emulator", playstationVersion), m_playstationVersion(playstationVersion) {
}

PlayStationEmulator::~PlayStationEmulator() = default;

bool PlayStationEmulator::initialize() {
    if (!BaseEmulator::initialize()) {
        return false;
    }
    
    return setupEmulator();
}

bool PlayStationEmulator::setupEmulator() {
    // Determine which emulator to use based on platform
    if (m_playstationVersion == "playstation4") {
        m_emulatorBinary = "rpcs4"; // Hypothetical PS4 emulator
        
        // Check if the emulator is installed
        int result = std::system("which rpcs4 > /dev/null 2>&1");
        if (result != 0) {
            std::cerr << "PS4 emulator (rpcs4) is not installed." << std::endl;
            return false;
        }
    } 
    else if (m_playstationVersion == "playstation5") {
        m_emulatorBinary = "rpcs5"; // Hypothetical PS5 emulator
        
        // Check if the emulator is installed
        int result = std::system("which rpcs5 > /dev/null 2>&1");
        if (result != 0) {
            std::cerr << "PS5 emulator (rpcs5) is not installed." << std::endl;
            return false;
        }
    }
    else {
        std::cerr << "Unsupported PlayStation version: " << m_playstationVersion << std::endl;
        return false;
    }
    
    std::cout << "PlayStation emulator configured: " << m_emulatorBinary << std::endl;
    return true;
}

int PlayStationEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize PlayStation emulator" << std::endl;
        return 1;
    }
    
    std::string gamePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();
    
    if (!fs::exists(gamePath)) {
        std::cerr << "Game file not found: " << gamePath << std::endl;
        return 1;
    }
    
    // Prepare command with emulator
    std::string command = m_emulatorBinary + " \"" + gamePath + "\"";
    
    std::cout << "Launching PlayStation game: " << gamePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    // Execute the command
    return std::system(command.c_str());
}

} // namespace XEmuRun
