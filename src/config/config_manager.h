#pragma once

#include "config.h"
#include <string>
#include <memory>
#include <map>
#include <filesystem>

namespace XEmuRun {

class ConfigManager {
public:
    static ConfigManager& getInstance();
    
    bool initialize();
    Config& getSystemConfig();
    Config& getEmulatorConfig(const std::string& platform);
    
    // Game-specific config management
    Config mergeWithGameConfig(const Config& gameConfig, const std::string& platform);
    
    // Configuration paths
    std::string getConfigDirectory() const;
    std::string getSystemConfigPath() const;
    std::string getEmulatorConfigPath(const std::string& platform) const;
    
    // Save configurations
    bool saveSystemConfig();
    bool saveEmulatorConfig(const std::string& platform);
    
    // Create default configurations
    void createDefaultSystemConfig();
    void createDefaultEmulatorConfig(const std::string& platform);
    
private:
    ConfigManager();
    ~ConfigManager();
    
    // Prevent copying
    ConfigManager(const ConfigManager&) = delete;
    ConfigManager& operator=(const ConfigManager&) = delete;
    
    // Configuration storage
    Config m_systemConfig;
    std::map<std::string, Config> m_emulatorConfigs;
    
    // Flag to track initialization
    bool m_initialized;
    
    // Helper methods
    bool ensureConfigDirectoryExists() const;
    bool loadConfig(Config& config, const std::string& path);
};

} // namespace XEmuRun
