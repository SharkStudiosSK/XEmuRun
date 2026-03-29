#include "windows_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>

namespace fs = std::filesystem;

namespace XEmuRun {

WindowsEmulator::WindowsEmulator()
    : BaseEmulator("Proton-GE", "windows") {
}

WindowsEmulator::~WindowsEmulator() = default;

bool WindowsEmulator::initialize() {
    if (!BaseEmulator::initialize()) {
        return false;
    }
    
    return setupProton();
}

std::string WindowsEmulator::findProtonPath() {
    // First check config-specified path
    std::string configPath = m_config.getString("proton_path", "");
    if (!configPath.empty() && fs::exists(configPath + "/proton")) {
        return configPath;
    }

    // Search common Proton-GE installation directories
    std::vector<std::string> searchDirs;

    const char* home = std::getenv("HOME");
    if (home) {
        searchDirs.push_back(std::string(home) + "/.steam/root/compatibilitytools.d");
        searchDirs.push_back(std::string(home) + "/.local/share/Steam/compatibilitytools.d");
    }

    std::vector<std::string> foundPaths;

    for (const auto& dir : searchDirs) {
        if (!fs::exists(dir)) continue;

        for (const auto& entry : fs::directory_iterator(dir)) {
            if (!entry.is_directory()) continue;
            std::string name = entry.path().filename().string();
            // Look for GE-Proton directories (e.g. GE-Proton9-20)
            if (name.find("GE-Proton") == 0 || name.find("Proton-") == 0) {
                std::string protonBin = entry.path().string() + "/proton";
                if (fs::exists(protonBin)) {
                    foundPaths.push_back(entry.path().string());
                }
            }
        }
    }

    if (!foundPaths.empty()) {
        // Sort by parsed version number so the newest Proton-GE is always selected.
        // Directory names follow the pattern "GE-ProtonX-Y" (major=X, minor=Y).
        auto parseVersion = [](const std::string& path) -> std::pair<int,int> {
            std::string name = fs::path(path).filename().string();
            // Strip "GE-Proton" prefix
            const std::string prefix = "GE-Proton";
            if (name.find(prefix) == 0) {
                std::string rest = name.substr(prefix.size()); // e.g. "9-20" or "10-1"
                auto dash = rest.find('-');
                if (dash != std::string::npos) {
                    try {
                        int major = std::stoi(rest.substr(0, dash));
                        int minor = std::stoi(rest.substr(dash + 1));
                        return {major, minor};
                    } catch (...) {}
                }
            }
            return {0, 0};
        };

        std::sort(foundPaths.begin(), foundPaths.end(),
            [&parseVersion](const std::string& a, const std::string& b) {
                return parseVersion(a) < parseVersion(b);
            });
        return foundPaths.back();
    }

    return "";
}

bool WindowsEmulator::setupProton() {
    m_protonPath = findProtonPath();

    if (m_protonPath.empty()) {
        std::cerr << "Proton-GE is not installed. Please install Proton-GE to run Windows applications." << std::endl;
        std::cerr << "Download from: https://github.com/GloriousEggroll/proton-ge-custom/releases" << std::endl;
        std::cerr << "Install to: ~/.local/share/Steam/compatibilitytools.d/" << std::endl;
        return false;
    }

    std::cout << "Proton-GE detected at: " << m_protonPath << std::endl;

    const char* home = std::getenv("HOME");

    // Setup compatibility data path (equivalent to WINEPREFIX in Proton context)
    std::string compatDataPath = m_config.getString("proton_data_path", "");
    if (compatDataPath.empty() && home) {
        compatDataPath = std::string(home) + "/.local/share/XEmuRun/compatdata";
    }

    if (!compatDataPath.empty()) {
        fs::create_directories(compatDataPath);
        setenv("STEAM_COMPAT_DATA_PATH", compatDataPath.c_str(), 1);
        std::cout << "Using Proton compatibility data path: " << compatDataPath << std::endl;
    }

    // Set Steam client install path required by Proton
    if (home) {
        std::string steamPath = std::string(home) + "/.local/share/Steam";
        if (!fs::exists(steamPath)) {
            steamPath = std::string(home) + "/.steam/steam";
        }
        setenv("STEAM_COMPAT_CLIENT_INSTALL_PATH", steamPath.c_str(), 1);
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
    
    // Build the Proton-GE launch command
    std::string command = "\"" + m_protonPath + "/proton\" run";
    command += " \"" + executablePath + "\"";
    
    std::cout << "Launching Windows application with Proton-GE: " << executablePath << std::endl;
    std::cout << "Command: " << command << std::endl;
    
    return std::system(command.c_str());
}

Config WindowsEmulator::getDefaultConfig() const {
    Config config = BaseEmulator::getDefaultConfig();
    
    // Proton-GE-specific settings
    config.setString("proton_path", "");
    config.setString("proton_data_path", "");
    config.setInt("windows_version", 10);
    
    return config;
}

void WindowsEmulator::applyConfig(const Config& config) {
    BaseEmulator::applyConfig(config);
    
    // Apply Proton-GE-specific settings
    std::string protonPath = config.getString("proton_path", "");
    if (!protonPath.empty()) {
        std::cout << "Using Proton-GE path: " << protonPath << std::endl;
    }

    std::string protonDataPath = config.getString("proton_data_path", "");
    if (!protonDataPath.empty()) {
        std::cout << "Using Proton compatibility data path: " << protonDataPath << std::endl;
    }
    
    int windowsVersion = config.getInt("windows_version", 10);
    std::cout << "Windows version set to: Windows " << windowsVersion << std::endl;
}

} // namespace XEmuRun
