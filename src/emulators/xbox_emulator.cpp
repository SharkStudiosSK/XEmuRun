#include "xbox_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <fstream>
#include <thread>
#include <map>

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
    
    // Determine which emulator to use based on Xbox version
    if (m_xboxVersion == "xbox") {
        return setupOriginalXboxEmulator();
    } 
    else if (m_xboxVersion == "xbox_360") {
        return setupXbox360Emulator();
    }
    else if (m_xboxVersion == "xbox_one") {
        return setupXboxOneEmulator();
    }
    else if (m_xboxVersion == "xbox_series") {
        return setupXboxSeriesEmulator();
    }
    else {
        std::cerr << "Unsupported Xbox version: " << m_xboxVersion << std::endl;
        return false;
    }
}

bool XboxEmulator::setupOriginalXboxEmulator() {
    m_emulatorBinary = findEmulatorPath("xemu");
    
    // Check if the emulator is installed
    if (m_emulatorBinary.empty()) {
        std::cerr << "Original Xbox emulator (xemu) is not installed." << std::endl;
        return false;
    }
    
    // Check for BIOS files
    m_biosPath = m_config.getString("xbox_bios_path", "");
    if (m_biosPath.empty() || !fs::exists(m_biosPath)) {
        std::cerr << "Warning: Xbox BIOS not found. Original Xbox BIOS may be required." << std::endl;
    }
    
    std::cout << "Original Xbox emulator configured: " << m_emulatorBinary << std::endl;
    return true;
}

bool XboxEmulator::setupXbox360Emulator() {
    m_emulatorBinary = findEmulatorPath("xenia");
    
    // Check if the emulator is installed
    if (m_emulatorBinary.empty()) {
        std::cerr << "Xbox 360 emulator (xenia) is not installed." << std::endl;
        return false;
    }
    
    std::cout << "Xbox 360 emulator configured: " << m_emulatorBinary << std::endl;
    return true;
}

bool XboxEmulator::setupXboxOneEmulator() {
    // Xbox One emulation is still very experimental
    m_emulatorBinary = findEmulatorPath("xbox1_emu");
    
    if (m_emulatorBinary.empty()) {
        std::cerr << "Xbox One emulator is not installed or not supported yet." << std::endl;
        return false;
    }
    
    std::cout << "Xbox One emulator configured: " << m_emulatorBinary << std::endl;
    std::cout << "Warning: Xbox One emulation is highly experimental" << std::endl;
    return true;
}

bool XboxEmulator::setupXboxSeriesEmulator() {
    // Xbox Series emulation is still theoretical
    m_emulatorBinary = findEmulatorPath("xbsx_emu");
    
    if (m_emulatorBinary.empty()) {
        std::cerr << "Xbox Series emulator is not installed or not supported yet." << std::endl;
        return false;
    }
    
    std::cout << "Xbox Series emulator configured: " << m_emulatorBinary << std::endl;
    std::cout << "Warning: Xbox Series emulation is highly experimental" << std::endl;
    return true;
}

std::string XboxEmulator::findEmulatorPath(const std::string& defaultName) {
    // Try to find emulator in system path
    int result = std::system(("which " + defaultName + " > /dev/null 2>&1").c_str());
    if (result == 0) {
        // Get full path
        std::string cmd = "which " + defaultName;
        FILE* pipe = popen(cmd.c_str(), "r");
        if (!pipe) {
            return defaultName;
        }
        
        char buffer[128];
        std::string path = "";
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                path += buffer;
        }
        pclose(pipe);
        
        // Remove trailing newline
        if (!path.empty() && path[path.length()-1] == '\n') {
            path.erase(path.length()-1);
        }
        
        return path;
    }
    
    // Check if user has specified a custom path in config
    std::string customPath = m_config.getString(m_xboxVersion + "_emulator_path", "");
    if (!customPath.empty() && fs::exists(customPath)) {
        return customPath;
    }
    
    // Try common installation locations
    std::map<std::string, std::vector<std::string>> searchPaths = {
        {"xemu", {
            "/usr/bin/xemu",
            "/usr/local/bin/xemu",
            "/opt/xemu/bin/xemu"
        }},
        {"xenia", {
            "/usr/bin/xenia",
            "/usr/local/bin/xenia",
            "/opt/xenia/xenia"
        }},
        {"xbox1_emu", {
            "/usr/bin/xbox1_emu",
            "/usr/local/bin/xbox1_emu",
            "/opt/xbox1_emu/xbox1_emu"
        }},
        {"xbsx_emu", {
            "/usr/bin/xbsx_emu",
            "/usr/local/bin/xbsx_emu",
            "/opt/xbsx_emu/xbsx_emu"
        }}
    };
    
    if (searchPaths.find(defaultName) != searchPaths.end()) {
        for (const auto& path : searchPaths[defaultName]) {
            if (fs::exists(path)) {
                return path;
            }
        }
    }
    
    // If we get here, we couldn't find the emulator
    return "";
}

int XboxEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize Xbox emulator" << std::endl;
        return 1;
    }
    
    if (m_emulatorBinary.empty()) {
        std::cerr << "No emulator binary configured" << std::endl;
        return 1;
    }
    
    std::string gamePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();
    
    if (!fs::exists(gamePath)) {
        std::cerr << "Game file not found: " << gamePath << std::endl;
        return 1;
    }
    
    // Prepare command with emulator-specific options
    std::string command = m_emulatorBinary;
    
    // Add version-specific arguments
    if (m_xboxVersion == "xbox") {
        if (!m_biosPath.empty()) {
            command += " --bios \"" + m_biosPath + "\"";
        }
        
        if (m_config.getBool("hdd_enabled", true)) {
            std::string hddPath = m_config.getString("hdd_path", "");
            if (!hddPath.empty()) {
                command += " --hdd \"" + hddPath + "\"";
            }
        }
    } 
    else if (m_xboxVersion == "xbox_360") {
        // Xenia specific arguments
        if (m_config.getBool("fullscreen", true)) {
            command += " --fullscreen";
        }
        
        if (m_config.getBool("vsync", true)) {
            command += " --vsync";
        }
    }
    else if (m_xboxVersion == "xbox_one" || m_xboxVersion == "xbox_series") {
        // Future emulator arguments would go here
        command += " --experimental";
    }
    
    // Add game path (this is always the last argument)
    command += " \"" + gamePath + "\"";
    
    std::cout << "Launching " << m_xboxVersion << " game: " << gamePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    // Execute the command
    return std::system(command.c_str());
}

Config XboxEmulator::getDefaultConfig() const {
    Config config = BaseEmulator::getDefaultConfig();
    
    // Common settings for all Xbox emulators
    config.setBool("fullscreen", true);
    config.setBool("vsync", true);
    
    // Version-specific settings
    if (m_xboxVersion == "xbox") {
        // Original Xbox
        config.setString("xbox_bios_path", "");
        config.setBool("hdd_enabled", true);
        config.setString("hdd_path", "");
    } 
    else if (m_xboxVersion == "xbox_360") {
        // Xbox 360
        config.setBool("gpu_hardware_acceleration", true);
        config.setString("save_directory", "");
    }
    else if (m_xboxVersion == "xbox_one") {
        // Xbox One
        config.setBool("experimental_mode", true);
        config.setInt("rendering_scale", 75);
    }
    else if (m_xboxVersion == "xbox_series") {
        // Xbox Series X|S
        config.setBool("experimental_mode", true);
        config.setInt("rendering_scale", 50);
        config.setBool("raytracing", false);
    }
    
    return config;
}

} // namespace XEmuRun
