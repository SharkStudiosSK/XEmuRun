#include "packager.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include "../utils/archive.h"

namespace fs = std::filesystem;

namespace XEmuRun {

Packager::Packager() = default;
Packager::~Packager() = default;

void Packager::setGamePath(const std::string& path) {
    m_gamePath = path;
}

void Packager::setOutputPath(const std::string& path) {
    m_outputPath = path;
}

void Packager::setGameName(const std::string& name) {
    m_gameName = name;
}

void Packager::setPlatform(const std::string& platform) {
    m_platform = platform;
}

void Packager::setMainExecutable(const std::string& executable) {
    m_mainExecutable = executable;
}

void Packager::addConfigValue(const std::string& key, const std::string& value) {
    m_configValues[key] = value;
}

bool Packager::createPackage() {
    if (m_gamePath.empty() || m_outputPath.empty() || m_gameName.empty() || 
        m_platform.empty() || m_mainExecutable.empty()) {
        std::cerr << "Missing required packaging information" << std::endl;
        return false;
    }
    
    // Validate input
    if (!validateInputs()) {
        std::cerr << "Input validation failed" << std::endl;
        return false;
    }
    
    // Create temp directory for packaging
    std::string tempDir = (fs::temp_directory_path() / ("XEmuPkg_" + m_gameName)).string();
    
    try {
        if (fs::exists(tempDir)) {
            fs::remove_all(tempDir);
        }
        fs::create_directories(tempDir);
        
        // Copy game files
        std::cout << "Copying game files..." << std::endl;
        fs::copy(m_gamePath, tempDir + "/game", fs::copy_options::recursive);
        
        // Generate manifest
        std::string manifestPath = tempDir + "/manifest.json";
        if (!generateManifest()) {
            std::cerr << "Failed to generate manifest" << std::endl;
            return false;
        }
        
        // Create the actual package
        std::string packagePath = (fs::path(m_outputPath) / (m_gameName + ".XEmupkg")).string();
        
        // Ensure output directory exists
        fs::create_directories(fs::path(packagePath).parent_path());
        
        std::cout << "Creating package archive..." << std::endl;
        if (!createArchive(tempDir, packagePath)) {
            std::cerr << "Failed to create package archive" << std::endl;
            return false;
        }
        
        std::cout << "Package created successfully: " << packagePath << std::endl;
        
        // Clean up temp files
        std::cout << "Cleaning up temporary files..." << std::endl;
        fs::remove_all(tempDir);
        
        return true;
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Packaging error: " << e.what() << std::endl;
        return false;
    }
}

bool Packager::generateManifest() {
    Json::Value root;
    root["name"] = m_gameName;
    root["platform"] = m_platform;
    root["main"] = m_mainExecutable;
    root["version"] = "1.0.0";  // Add version information
    root["created"] = getCurrentTimestamp();
    
    // Add configuration values
    Json::Value config;
    for (const auto& [key, value] : m_configValues) {
        config[key] = value;
    }
    root["config"] = config;
    
    // Write manifest to file
    std::string manifestPath = (fs::path(fs::temp_directory_path()) / 
                                ("XEmuPkg_" + m_gameName) / 
                                "manifest.json").string();
    
    std::ofstream file(manifestPath);
    if (!file.is_open()) {
        std::cerr << "Failed to open manifest file for writing" << std::endl;
        return false;
    }
    
    Json::StyledWriter writer;
    file << writer.write(root);
    file.close();
    
    return true;
}

bool Packager::validateInputs() {
    // Check if game path exists
    if (!fs::exists(m_gamePath)) {
        std::cerr << "Game path does not exist: " << m_gamePath << std::endl;
        return false;
    }
    
    // Check if game path is a directory
    if (!fs::is_directory(m_gamePath)) {
        std::cerr << "Game path is not a directory: " << m_gamePath << std::endl;
        return false;
    }
    
    // Check if main executable exists within the game path
    std::string executablePath = (fs::path(m_gamePath) / m_mainExecutable).string();
    if (!fs::exists(executablePath)) {
        std::cerr << "Main executable not found: " << executablePath << std::endl;
        return false;
    }
    
    // Validate platform
    std::vector<std::string> validPlatforms = {
        "windows", "linux", "playstation4", "playstation5", "xbox", "xbox_series"
    };
    
    bool validPlatform = false;
    for (const auto& platform : validPlatforms) {
        if (m_platform == platform) {
            validPlatform = true;
            break;
        }
    }
    
    if (!validPlatform) {
        std::cerr << "Invalid platform: " << m_platform << std::endl;
        std::cerr << "Valid platforms are: windows, linux, playstation4, playstation5, xbox, xbox_series" << std::endl;
        return false;
    }
    
    return true;
}

std::string Packager::getCurrentTimestamp() {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

bool Packager::packageFiles() {
    // This is now handled directly in createPackage()
    return true;
}

} // namespace XEmuRun
