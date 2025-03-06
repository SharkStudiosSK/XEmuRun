#include "windows_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>

namespace fs = std::filesystem;

namespace XEmuRun {

WindowsEmulator::WindowsEmulator()
    : BaseEmulator("Wine", "windows") {
}

WindowsEmulator::~WindowsEmulator() = default;

bool WindowsEmulator::initialize() {
    if (!BaseEmulator::initialize()) {
        return false;
    }
    
    return setupWine();
}

bool WindowsEmulator::setupWine() {
    // Check if Wine is installed
    int result = std::system("which wine > /dev/null 2>&1");
    if (result != 0) {
        std::cerr << "Wine is not installed. Please install Wine to run Windows applications." << std::endl;
        return false;
    }
    
    std::cout << "Wine detected successfully." << std::endl;
    
    // Setup Wine environment if custom Wine prefix is specified
    std::string winePrefix = m_config.getString("wine_prefix", "");
    if (!winePrefix.empty()) {
        std::cout << "Using custom Wine prefix: " << winePrefix << std::endl;
        setenv("WINEPREFIX", winePrefix.c_str(), 1);
    }
    
    // Configure DXVK if enabled
    bool enableDxvk = m_config.getBool("enable_dxvk", true);
    if (enableDxvk) {
        std::cout << "DXVK is enabled for DirectX support" << std::endl;
        setenv("WINEDLLOVERRIDES", "d3d11,d3d10,d3d9=n", 1);
    }
    
    return true;
}

int WindowsEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize Windows emulator" << std::endl;
        return 1;
    }
    
    std::string executablePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();
    
    if (!fs::exists(executablePath)) {
        std::cerr << "Executable not found: " << executablePath << std::endl;
        return 1;
    }
    
    // Prepare Wine command with configuration
    std::string command = "wine";
    
    // Add configuration parameters
    bool fullscreen = m_config.getBool("fullscreen", true);
    if (fullscreen) {
        command += " explorer /desktop=XEmuRun,";
        command += std::to_string(m_config.getInt("resolution_width", 1920));
        command += "x";
        command += std::to_string(m_config.getInt("resolution_height", 1080));
    }
    
    // Add executable path
    command += " \"" + executablePath + "\"";
    
    std::cout << "Launching Windows application: " << executablePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    // Execute the command
    return std::system(command.c_str());
}

Config WindowsEmulator::getDefaultConfig() const {
    Config config = BaseEmulator::getDefaultConfig();
    
    // Windows-specific settings
    config.setString("wine_prefix", "");
    config.setString("wine_version", "");
    config.setBool("enable_dxvk", true);
    config.setInt("windows_version", 10);
    
    return config;
}

void WindowsEmulator::applyConfig(const Config& config) {
    BaseEmulator::applyConfig(config);
    
    // Apply Windows-specific settings
    std::string winePrefix = config.getString("wine_prefix", "");
    if (!winePrefix.empty()) {
        std::cout << "Using Wine prefix: " << winePrefix << std::endl;
    }
    
    bool enableDxvk = config.getBool("enable_dxvk", true);
    std::cout << "DXVK " << (enableDxvk ? "enabled" : "disabled") << std::endl;
    
    int windowsVersion = config.getInt("windows_version", 10);
    std::cout << "Windows version set to: Windows " << windowsVersion << std::endl;
}

} // namespace XEmuRun
