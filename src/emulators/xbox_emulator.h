#pragma once

#include "base_emulator.h"
#include "../package/package.h"
#include <string>

namespace XEmuRun {

class XboxEmulator : public BaseEmulator {
public:
    explicit XboxEmulator(const std::string& xboxVersion);
    ~XboxEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    Config getDefaultConfig() const override;
    
private:
    bool setupOriginalXboxEmulator();
    bool setupXbox360Emulator();
    bool setupXboxOneEmulator();
    bool setupXboxSeriesEmulator();
    std::string findEmulatorPath(const std::string& defaultName);
    
    std::string m_emulatorBinary;
    std::string m_xboxVersion;
    std::string m_biosPath;
};

} // namespace XEmuRun
