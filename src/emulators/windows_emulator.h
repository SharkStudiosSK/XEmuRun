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
    std::string m_protonPath;
    
    std::string findProtonPath();
    bool setupProton();

    // Returns true if the named command exists somewhere on PATH
    static bool commandAvailable(const std::string& cmd);

    // Replace characters that are unsafe in filesystem paths or environment values
    static std::string sanitizeId(const std::string& input);
};

} // namespace XEmuRun
