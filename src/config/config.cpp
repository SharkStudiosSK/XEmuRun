#include "config.h"
#include <fstream>
#include <iostream>

namespace XEmuRun {

Config::Config() = default;
Config::~Config() = default;

void Config::loadFromJson(const Json::Value& root) {
    // Process string values
    for (auto it = root.begin(); it != root.end(); ++it) {
        const std::string& key = it.key().asString();
        const Json::Value& value = *it;
        
        if (value.isString()) {
            m_stringValues[key] = value.asString();
        } else if (value.isInt()) {
            m_intValues[key] = value.asInt();
        } else if (value.isBool()) {
            m_boolValues[key] = value.asBool();
        }
    }
}

void Config::loadFromFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open config file: " << path << std::endl;
        return;
    }
    
    Json::Value root;
    Json::Reader reader;
    
    if (!reader.parse(file, root)) {
        std::cerr << "Failed to parse config JSON from file: " << path << std::endl;
        return;
    }
    
    loadFromJson(root);
}

std::string Config::getString(const std::string& key, const std::string& defaultValue) const {
    auto it = m_stringValues.find(key);
    return (it != m_stringValues.end()) ? it->second : defaultValue;
}

int Config::getInt(const std::string& key, int defaultValue) const {
    auto it = m_intValues.find(key);
    return (it != m_intValues.end()) ? it->second : defaultValue;
}

bool Config::getBool(const std::string& key, bool defaultValue) const {
    auto it = m_boolValues.find(key);
    return (it != m_boolValues.end()) ? it->second : defaultValue;
}

void Config::setString(const std::string& key, const std::string& value) {
    m_stringValues[key] = value;
}

void Config::setInt(const std::string& key, int value) {
    m_intValues[key] = value;
}

void Config::setBool(const std::string& key, bool value) {
    m_boolValues[key] = value;
}

void Config::save(const std::string& path) {
    Json::Value root;
    
    // Add string values
    for (const auto& [key, value] : m_stringValues) {
        root[key] = value;
    }
    
    // Add int values
    for (const auto& [key, value] : m_intValues) {
        root[key] = value;
    }
    
    // Add bool values
    for (const auto& [key, value] : m_boolValues) {
        root[key] = value;
    }
    
    // Write to file
    std::ofstream file(path);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for writing: " << path << std::endl;
        return;
    }
    
    Json::StyledWriter writer;
    file << writer.write(root);
}

} // namespace XEmuRun
