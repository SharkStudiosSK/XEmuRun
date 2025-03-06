#pragma once

#include <string>
#include "../config/config.h"

namespace XEmuRun {

class Package;

class EmulatorInterface {
public:
    virtual ~EmulatorInterface() = default;
    
    virtual bool initialize() = 0;
    virtual int launch(const Package& package) = 0;
    virtual std::string getName() const = 0;
    virtual std::string getSupportedPlatform() const = 0;
    
    // Configuration methods
    virtual void applyConfig(const Config& config) = 0;
    virtual Config getDefaultConfig() const = 0;
};

} // namespace XEmuRun
