#include "package.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include "../utils/archive.h"

namespace fs = std::filesystem;

namespace XEmuRun {

Package::Package() = default;
Package::~Package() = default;

bool Package::load(const std::string& packagePath) {
    m_packagePath = packagePath;
    
    if (!validatePackage()) {
        std::cerr << "Package validation failed" << std::endl;
        return false;
    }
    
    if (!extractPackage()) {
        std::cerr << "Package extraction failed" << std::endl;
        return false;
    }
    
    if (!loadManifest()) {
        std::cerr << "Failed to load package manifest" << std::endl;
        return false;
    }
    
    return true;
}

bool Package::extract() {
    return true; // Already extracted during load
}

std::string Package::getName() const {
    return m_name;
}

std::string Package::getPlatform() const {
    return m_platform;
}

std::string Package::getMainExecutable() const {
    return m_mainExecutable;
}

std::string Package::getExtractedPath() const {
    return m_extractedPath;
}

const Config& Package::getConfig() const {
    return m_config;
}

bool Package::validatePackage() {
    if (!fs::exists(m_packagePath)) {
        std::cerr << "Package file does not exist: " << m_packagePath << std::endl;
        return false;
    }
    
    if (fs::path(m_packagePath).extension() != ".XEmupkg") {
        std::cerr << "File is not an XEmupkg: " << m_packagePath << std::endl;
        return false;
    }
    
    return true;
}

bool Package::extractPackage() {
    // Create a temporary directory for extraction
    m_extractedPath = fs::temp_directory_path() / "XEmuRun" / fs::path(m_packagePath).stem().string();
    
    try {
        fs::create_directories(m_extractedPath);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Failed to create extraction directory: " << e.what() << std::endl;
        return false;
    }
    
    // Extract the package
    if (!extractArchive(m_packagePath, m_extractedPath)) {
        std::cerr << "Failed to extract package" << std::endl;
        return false;
    }
    
    return true;
}

bool Package::loadManifest() {
    std::string manifestPath = (fs::path(m_extractedPath) / "manifest.json").string();
    
    if (!fs::exists(manifestPath)) {
        std::cerr << "Manifest file not found: " << manifestPath << std::endl;
        return false;
    }
    
    std::ifstream file(manifestPath);
    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(file, root)) {
        std::cerr << "Failed to parse manifest JSON" << std::endl;
        return false;
    }
    
    m_name = root["name"].asString();
    m_platform = root["platform"].asString();
    m_mainExecutable = root["main"].asString();
    
    // Load configuration
    if (root.isMember("config")) {
        m_config.loadFromJson(root["config"]);
    }
    
    return true;
}

} // namespace XEmuRun
