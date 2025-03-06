#pragma once

#include "base_emulator.h"
#include "../package/package.h"

namespace XEmuRun {

class PlayStationEmulator : public BaseEmulator {
public:
    explicit PlayStationEmulator(const std::string& playstationVersion);
    ~PlayStationEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    
private:
    bool setupEmulator();
    std::string m_emulatorBinary;
    std::string m_playstationVersion;
};

} // namespace XEmuRun
