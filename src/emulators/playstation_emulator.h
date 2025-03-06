#pragma once

#include "base_emulator.h"
#include "../package/package.h"
#include <string>

namespace XEmuRun {

// Forward declarations for PlayStation emulator cores
class XEmuPS4Core;
class XEmuPS5Core;

/**
 * @class PlayStationEmulator
 * @brief Custom PlayStation emulator implementation for XEmuRun.
 * 
 * This emulator provides support for PlayStation 4 and PlayStation 5 games
 * through custom emulation cores.
 */
class PlayStationEmulator : public BaseEmulator {
public:
    explicit PlayStationEmulator(const std::string& playstationVersion);
    ~PlayStationEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    Config getDefaultConfig() const override;
    
private:
    std::string m_playstationVersion;
    std::string m_biosPath;
    
    // Emulation cores
    XEmuPS4Core* m_ps4Core;
    XEmuPS5Core* m_ps5Core;
};

} // namespace XEmuRun
