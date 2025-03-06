#pragma once

#include "base_emulator.h"
#include "../package/package.h"

namespace XEmuRun {

class WindowsEmulator : public BaseEmulator {
public:
    WindowsEmulator();
    ~WindowsEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    
    // Configuration methods
    void applyConfig(const Config& config) override;
    Config getDefaultConfig() const override;
    
private:
    bool setupWine();
};

} // namespace XEmuRun
