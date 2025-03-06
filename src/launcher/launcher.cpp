#include "launcher.h"
#include "../emulators/windows_emulator.h"
#include "../emulators/linux_emulator.h"
#include "../emulators/playstation_emulator.h"
#include "../emulators/xbox_emulator.h"
#include "../config/config_manager.h"
#include <iostream>

namespace XEmuRun {

Launcher::Launcher() = default;
Launcher::~Launcher() = default;

bool Launcher::loadPackage(const std::string& packagePath) {
    // Initialize configuration system
    ConfigManager& configManager = ConfigManager::getInstance();
    if (!configManager.initialize()) {
        std::cerr << "Failed to initialize configuration system" << std::endl;
        return false;
    }
    
    m_currentPackage = std::make_unique<Package>();
    if (!m_currentPackage->load(packagePath)) {
        std::cerr << "Failed to load package: " << packagePath << std::endl;
        return false;
    }
    
    std::cout << "Successfully loaded package: " << m_currentPackage->getName() << std::endl;
    std::cout << "Platform: " << m_currentPackage->getPlatform() << std::endl;
    
    // Create appropriate emulator for the package
    m_emulator.reset(createEmulatorForPlatform(m_currentPackage->getPlatform()));
    
    if (!m_emulator) {
        std::cerr << "Unsupported platform: " << m_currentPackage->getPlatform() << std::endl;
        return false;
    }
    
    // Merge system, emulator, and game configurations
    Config gameConfig = m_currentPackage->getConfig();
    Config mergedConfig = configManager.mergeWithGameConfig(gameConfig, m_currentPackage->getPlatform());
    
    // Apply the merged configuration
    m_emulator->applyConfig(mergedConfig);
    
    return true;
}

int Launcher::runGame() {
    if (!m_currentPackage || !m_emulator) {
        std::cerr << "No package loaded or emulator created" << std::endl;
        return 1;
    }
    
    std::cout << "Starting emulation..." << std::endl;
    return m_emulator->launch(*m_currentPackage);
}

EmulatorInterface* Launcher::createEmulatorForPlatform(const std::string& platform) {
    if (platform == "windows") {
        return new WindowsEmulator();
    } else if (platform == "linux") {
        return new LinuxEmulator();
    } else if (platform == "playstation4" || platform == "playstation5") {
        return new PlayStationEmulator(platform);
    } else if (platform == "xbox" || platform == "xbox_series") {
        return new XboxEmulator(platform);
    }
    
    return nullptr;
}

} // namespace XEmuRun
