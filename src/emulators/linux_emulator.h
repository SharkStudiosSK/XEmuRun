#pragma once

#include "base_emulator.h"
#include "../package/package.h"

namespace XEmuRun {

class LinuxEmulator : public BaseEmulator {
public:
    LinuxEmulator();
    ~LinuxEmulator() override;
    
    bool initialize() override;
    int launch(const Package& package) override;
    
private:
    bool setupEnvironment();
};

} // namespace XEmuRun
