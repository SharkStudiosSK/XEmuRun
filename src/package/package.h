#pragma once

#include <string>
#include <vector>
#include <map>
#include "../config/config.h"

namespace XEmuRun {

class Package {
public:
    Package();
    ~Package();
    
    bool load(const std::string& packagePath);
    bool extract();
    
    std::string getName() const;
    std::string getPlatform() const;
    std::string getMainExecutable() const;
    std::string getExtractedPath() const;
    const Config& getConfig() const;
    
private:
    std::string m_packagePath;
    std::string m_extractedPath;
    std::string m_name;
    std::string m_platform;
    std::string m_mainExecutable;
    Config m_config;
    
    bool validatePackage();
    bool extractPackage();
    bool loadManifest();
};

} // namespace XEmuRun
