#include "base_emulator.h"

namespace XEmuRun {

BaseEmulator::BaseEmulator(const std::string& name, const std::string& platform)
    : m_name(name), m_platform(platform), m_initialized(false) {
}

bool BaseEmulator::initialize() {
    std::cout << "Initializing " << m_name << " emulator..." << std::endl;
    m_initialized = true;
    return true;
}

std::string BaseEmulator::getName() const {
    return m_name;
}

std::string BaseEmulator::getSupportedPlatform() const {
    return m_platform;
}

void BaseEmulator::applyConfig(const Config& config) {
    // Store the configuration
    m_config = config;
    
    // Output some basic config info
    std::cout << "Applied configuration for " << m_name << " emulator" << std::endl;
    
    // Log some common settings
    bool fullscreen = config.getBool("fullscreen", true);
    int width = config.getInt("resolution_width", 1920);
    int height = config.getInt("resolution_height", 1080);
    
    std::cout << "Display mode: " << (fullscreen ? "Fullscreen" : "Windowed") << std::endl;
    std::cout << "Resolution: " << width << "x" << height << std::endl;
}

Config BaseEmulator::getDefaultConfig() const {
    Config config;
    
    // Common settings for all emulators
    config.setBool("fullscreen", true);
    config.setInt("resolution_width", 1920);
    config.setInt("resolution_height", 1080);
    
    return config;
}

} // namespace XEmuRun
