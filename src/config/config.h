#pragma once

#include <string>
#include <map>
#include <json/json.h>

namespace XEmuRun {

class ConfigManager; // Forward declaration

class Config {
public:
    Config();
    ~Config();
    
    void loadFromJson(const Json::Value& root);
    void loadFromFile(const std::string& path);
    
    std::string getString(const std::string& key, const std::string& defaultValue = "") const;
    int getInt(const std::string& key, int defaultValue = 0) const;
    bool getBool(const std::string& key, bool defaultValue = false) const;
    
    void setString(const std::string& key, const std::string& value);
    void setInt(const std::string& key, int value);
    void setBool(const std::string& key, bool value);
    
    void save(const std::string& path);
    
private:
    std::map<std::string, std::string> m_stringValues;
    std::map<std::string, int> m_intValues;
    std::map<std::string, bool> m_boolValues;
    
    // Grant ConfigManager access to private members for merging configs
    friend class ConfigManager;
};

} // namespace XEmuRun
