#pragma once

#include <string>
#include <vector>
#include <map>
#include <chrono>
#include <iomanip>
#include <sstream>

namespace XEmuRun {

class Packager {
public:
    Packager();
    ~Packager();
    
    void setGamePath(const std::string& path);
    void setOutputPath(const std::string& path);
    void setGameName(const std::string& name);
    void setPlatform(const std::string& platform);
    void setMainExecutable(const std::string& executable);
    void addConfigValue(const std::string& key, const std::string& value);
    
    // Accessor methods
    std::string getGamePath() const { return m_gamePath; }
    std::string getOutputPath() const { return m_outputPath; }
    std::string getGameName() const { return m_gameName; }
    std::string getPlatform() const { return m_platform; }
    std::string getMainExecutable() const { return m_mainExecutable; }
    const std::map<std::string, std::string>& getConfigValues() const { return m_configValues; }
    
    bool createPackage();
    
private:
    std::string m_gamePath;
    std::string m_outputPath;
    std::string m_gameName;
    std::string m_platform;
    std::string m_mainExecutable;
    std::map<std::string, std::string> m_configValues;
    
    bool generateManifest();
    bool packageFiles();
    bool validateInputs();
    std::string getCurrentTimestamp();
};

} // namespace XEmuRun
