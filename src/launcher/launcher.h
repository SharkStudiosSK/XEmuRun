#pragma once

#include <string>
#include <memory>
#include "../package/package.h"
#include "../emulators/emulator_interface.h"

namespace XEmuRun {

class Launcher {
public:
    Launcher();
    ~Launcher();
    
    bool loadPackage(const std::string& packagePath);
    int runGame();
    
private:
    std::unique_ptr<Package> m_currentPackage;
    std::unique_ptr<EmulatorInterface> m_emulator;
    
    EmulatorInterface* createEmulatorForPlatform(const std::string& platform);
};

} // namespace XEmuRun
