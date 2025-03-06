#pragma once

#include "emulator_interface.h"
#include <string>
#include <iostream>

namespace XEmuRun {

class BaseEmulator : public EmulatorInterface {
public:
    BaseEmulator(const std::string& name, const std::string& platform);
    virtual ~BaseEmulator() = default;
    
    virtual bool initialize() override;
    virtual std::string getName() const override;
    virtual std::string getSupportedPlatform() const override;
    
    // Configuration methods
    virtual void applyConfig(const Config& config) override;
    virtual Config getDefaultConfig() const override;
    
protected:
    std::string m_name;
    std::string m_platform;
    bool m_initialized;
    Config m_config;
};

} // namespace XEmuRun
