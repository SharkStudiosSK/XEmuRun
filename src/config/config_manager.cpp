#include "config_manager.h"
#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

namespace XEmuRun {

ConfigManager& ConfigManager::getInstance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager() : m_initialized(false) {
}

ConfigManager::~ConfigManager() {
    // Save any unsaved changes
    if (m_initialized) {
        saveSystemConfig();
        
        for (const auto& [platform, _] : m_emulatorConfigs) {
            saveEmulatorConfig(platform);
        }
    }
}

bool ConfigManager::initialize() {
    if (m_initialized) {
        return true;
    }
    
    // Ensure config directory exists
    if (!ensureConfigDirectoryExists()) {
        std::cerr << "Failed to create configuration directory" << std::endl;
        return false;
    }
    
    // Load system configuration
    std::string systemConfigPath = getSystemConfigPath();
    if (fs::exists(systemConfigPath)) {
        if (!loadConfig(m_systemConfig, systemConfigPath)) {
            std::cerr << "Failed to load system configuration" << std::endl;
            return false;
        }
    } else {
        // Create default system configuration
        createDefaultSystemConfig();
        saveSystemConfig();
    }
    
    // Load emulator configurations for known platforms
    std::vector<std::string> platforms = {
        "windows", "linux", "playstation4", "playstation5", "xbox", "xbox_series"
    };
    
    for (const auto& platform : platforms) {
        std::string emulatorConfigPath = getEmulatorConfigPath(platform);
        
        if (fs::exists(emulatorConfigPath)) {
            Config config;
            if (loadConfig(config, emulatorConfigPath)) {
                m_emulatorConfigs[platform] = config;
            } else {
                std::cerr << "Failed to load " << platform << " emulator configuration" << std::endl;
            }
        } else {
            // Create default emulator configuration
            createDefaultEmulatorConfig(platform);
            m_emulatorConfigs[platform] = Config();
            saveEmulatorConfig(platform);
        }
    }
    
    m_initialized = true;
    return true;
}

Config& ConfigManager::getSystemConfig() {
    return m_systemConfig;
}

Config& ConfigManager::getEmulatorConfig(const std::string& platform) {
    // Create configuration if it doesn't exist
    if (m_emulatorConfigs.find(platform) == m_emulatorConfigs.end()) {
        createDefaultEmulatorConfig(platform);
        saveEmulatorConfig(platform);
    }
    
    return m_emulatorConfigs[platform];
}

Config ConfigManager::mergeWithGameConfig(const Config& gameConfig, const std::string& platform) {
    // Start with the emulator's configuration
    Config mergedConfig = getEmulatorConfig(platform);
    
    // Override with game-specific settings
    // String values
    for (auto it = gameConfig.m_stringValues.begin(); it != gameConfig.m_stringValues.end(); ++it) {
        mergedConfig.setString(it->first, it->second);
    }
    
    // Integer values
    for (auto it = gameConfig.m_intValues.begin(); it != gameConfig.m_intValues.end(); ++it) {
        mergedConfig.setInt(it->first, it->second);
    }
    
    // Boolean values
    for (auto it = gameConfig.m_boolValues.begin(); it != gameConfig.m_boolValues.end(); ++it) {
        mergedConfig.setBool(it->first, it->second);
    }
    
    return mergedConfig;
}

std::string ConfigManager::getConfigDirectory() const {
    // Use XDG Base Directory specification on Linux
    const char* xdgConfigHome = std::getenv("XDG_CONFIG_HOME");
    
    if (xdgConfigHome) {
        return std::string(xdgConfigHome) + "/XEmuRun";
    } else {
        // Fallback to ~/.config/XEmuRun
        const char* home = std::getenv("HOME");
        if (home) {
            return std::string(home) + "/.config/XEmuRun";
        }
    }
    
    // Last resort, use a local directory
    return "./config";
}

std::string ConfigManager::getSystemConfigPath() const {
    return getConfigDirectory() + "/system.json";
}

std::string ConfigManager::getEmulatorConfigPath(const std::string& platform) const {
    return getConfigDirectory() + "/emulators/" + platform + ".json";
}

bool ConfigManager::saveSystemConfig() {
    std::string path = getSystemConfigPath();
    m_systemConfig.save(path);
    return true;
}

bool ConfigManager::saveEmulatorConfig(const std::string& platform) {
    if (m_emulatorConfigs.find(platform) == m_emulatorConfigs.end()) {
        std::cerr << "No configuration exists for platform: " << platform << std::endl;
        return false;
    }
    
    std::string path = getEmulatorConfigPath(platform);
    m_emulatorConfigs[platform].save(path);
    return true;
}

void ConfigManager::createDefaultSystemConfig() {
    m_systemConfig.setString("temp_directory", "");
    m_systemConfig.setString("default_output_directory", "");
    m_systemConfig.setBool("cleanup_temp_files", true);
    m_systemConfig.setInt("logging_level", 1); // 0=none, 1=errors, 2=warnings, 3=info, 4=debug
}

void ConfigManager::createDefaultEmulatorConfig(const std::string& platform) {
    Config& config = m_emulatorConfigs[platform];
    
    // Common settings for all emulators
    config.setString("game_directory", "");
    config.setBool("fullscreen", true);
    config.setInt("resolution_width", 1920);
    config.setInt("resolution_height", 1080);
    
    // Platform-specific default settings
    if (platform == "windows") {
        config.setString("wine_prefix", "");
        config.setString("wine_version", "");
        config.setBool("enable_dxvk", true);
        config.setInt("windows_version", 10); // Target Windows version (7, 8, 10)
    } 
    else if (platform == "playstation4" || platform == "playstation5") {
        config.setString("bios_path", "");
        config.setInt("rendering_resolution", 1080);
        config.setBool("vsync", true);
    }
    else if (platform == "xbox" || platform == "xbox_series") {
        config.setString("system_files_path", "");
        config.setBool("enable_hw_acceleration", true);
    }
}

bool ConfigManager::ensureConfigDirectoryExists() const {
    try {
        // Create main config directory
        std::string configDir = getConfigDirectory();
        if (!fs::exists(configDir)) {
            fs::create_directories(configDir);
        }
        
        // Create emulators subdirectory
        std::string emulatorsDir = configDir + "/emulators";
        if (!fs::exists(emulatorsDir)) {
            fs::create_directories(emulatorsDir);
        }
        
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "File system error: " << e.what() << std::endl;
        return false;
    }
}

bool ConfigManager::loadConfig(Config& config, const std::string& path) {
    try {
        config.loadFromFile(path);
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error loading config from " << path << ": " << e.what() << std::endl;
        return false;
    }
}

} // namespace XEmuRun
