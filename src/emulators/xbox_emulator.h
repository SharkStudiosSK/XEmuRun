#pragma once

#include "base_emulator.h"
#include "../package/package.h"

namespace XEmuRun {

class XboxEmulator : public BaseEmulator {
public:
    explicit XboxEmulator(const std::string& xboxVersion);
    ~XboxEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    
private:
    bool setupEmulator();
    std::string m_emulatorBinary;
    std::string m_xboxVersion;
};

} // namespace XEmuRun
