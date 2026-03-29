#include "windows_emulator.h"
#include <iostream>
#include <filesystem>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <cerrno>
#include <cstring>

namespace fs = std::filesystem;

namespace XEmuRun {

// ---------------------------------------------------------------------------
// Static helpers
// ---------------------------------------------------------------------------

bool WindowsEmulator::commandAvailable(const std::string& cmd) {
    const char* pathEnv = std::getenv("PATH");
    if (!pathEnv) return false;

    std::istringstream stream(pathEnv);
    std::string dir;
    while (std::getline(stream, dir, ':')) {
        if (dir.empty()) continue;
        std::string fullPath = dir + "/" + cmd;
        // Must be a regular file and executable by the current process
        if (fs::is_regular_file(fullPath) && access(fullPath.c_str(), X_OK) == 0) {
            return true;
        }
    }
    return false;
}

std::string WindowsEmulator::sanitizeId(const std::string& input) {
    // Characters unsafe in filesystem paths or environment variable values
    static const std::string unsafe = "/\\:*?\"<>|";
    std::string result = input;
    for (char& c : result) {
        if (unsafe.find(c) != std::string::npos) {
            c = '_';
        }
    }
    return result;
}

// ---------------------------------------------------------------------------
// Construction / destruction
// ---------------------------------------------------------------------------

WindowsEmulator::WindowsEmulator()
    : BaseEmulator("Proton-GE", "windows") {
}

WindowsEmulator::~WindowsEmulator() = default;

// ---------------------------------------------------------------------------
// initialize / setupProton / findProtonPath
// ---------------------------------------------------------------------------

bool WindowsEmulator::initialize() {
    // Print the standard initialization message, but manage m_initialized
    // ourselves so that a setupProton() failure doesn't leave it true.
    std::cout << "Initializing " << m_name << " emulator..." << std::endl;

    if (!setupProton()) {
        return false;
    }

    m_initialized = true;
    return true;
}

std::string WindowsEmulator::findProtonPath() {
    // First check config-specified path
    std::string configPath = m_config.getString("proton_path", "");
    if (!configPath.empty() && fs::exists(configPath + "/proton")) {
        return configPath;
    }

    // Search common Proton-GE installation directories
    std::vector<std::string> searchDirs;

    const char* home = std::getenv("HOME");
    if (home) {
        searchDirs.push_back(std::string(home) + "/.steam/root/compatibilitytools.d");
        searchDirs.push_back(std::string(home) + "/.local/share/Steam/compatibilitytools.d");
    }

    std::vector<std::string> foundPaths;

    for (const auto& dir : searchDirs) {
        if (!fs::exists(dir)) continue;

        try {
            for (const auto& entry : fs::directory_iterator(dir)) {
                if (!entry.is_directory()) continue;
                std::string name = entry.path().filename().string();
                // Only match GE-Proton directories (e.g. GE-Proton9-20, GE-Proton10-1).
                // The "Proton-*" naming scheme is intentionally excluded because its version
                // format differs and would corrupt the numeric sort below.
                if (name.find("GE-Proton") == 0) {
                    std::string protonBin = entry.path().string() + "/proton";
                    if (fs::exists(protonBin)) {
                        foundPaths.push_back(entry.path().string());
                    }
                }
            }
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Warning: failed to scan directory '" << dir
                      << "': " << e.what() << std::endl;
        }
    }

    if (!foundPaths.empty()) {
        // Sort by parsed version number (GE-ProtonX-Y) so the newest version wins.
        // Alphabetical sorting would incorrectly rank GE-Proton10-1 < GE-Proton9-20.
        auto parseVersion = [](const std::string& path) -> std::pair<int,int> {
            std::string name = fs::path(path).filename().string();
            const std::string prefix = "GE-Proton";
            if (name.find(prefix) == 0) {
                std::string rest = name.substr(prefix.size()); // e.g. "9-20" or "10-1"
                auto dash = rest.find('-');
                if (dash != std::string::npos) {
                    try {
                        int major = std::stoi(rest.substr(0, dash));
                        int minor = std::stoi(rest.substr(dash + 1));
                        return {major, minor};
                    } catch (...) {}
                }
            }
            return {0, 0};
        };

        std::sort(foundPaths.begin(), foundPaths.end(),
            [&parseVersion](const std::string& a, const std::string& b) {
                auto va = parseVersion(a);
                auto vb = parseVersion(b);
                if (va != vb) return va < vb;
                // Tie-break alphabetically for deterministic results
                return a < b;
            });
        return foundPaths.back();
    }

    return "";
}

bool WindowsEmulator::setupProton() {
    // Check that umu-launcher is available (required to run Proton outside Steam)
    if (!commandAvailable("umu-run")) {
        std::cerr << "umu-launcher (umu-run) is not installed. It is required to run Proton-GE standalone." << std::endl;
        std::cerr << "Install it from: https://github.com/Open-Wine-Components/umu-launcher" << std::endl;
        return false;
    }

    m_protonPath = findProtonPath();

    if (m_protonPath.empty()) {
        std::cerr << "Proton-GE is not installed. Please install Proton-GE to run Windows applications." << std::endl;
        std::cerr << "Download from: https://github.com/GloriousEggroll/proton-ge-custom/releases" << std::endl;
        std::cerr << "Install to: ~/.local/share/Steam/compatibilitytools.d/" << std::endl;
        return false;
    }

    std::cout << "Proton-GE detected at: " << m_protonPath << std::endl;

    // Export PROTONPATH so umu-run picks up the correct Proton-GE build
    setenv("PROTONPATH", m_protonPath.c_str(), 1);

    // Set Steam client install path only when a real Steam installation is found
    const char* home = std::getenv("HOME");
    if (home) {
        std::string primarySteamPath = std::string(home) + "/.local/share/Steam";
        std::string fallbackSteamPath = std::string(home) + "/.steam/steam";
        std::string steamPath;

        if (fs::exists(primarySteamPath)) {
            steamPath = primarySteamPath;
        } else if (fs::exists(fallbackSteamPath)) {
            steamPath = fallbackSteamPath;
        }

        if (!steamPath.empty()) {
            setenv("STEAM_COMPAT_CLIENT_INSTALL_PATH", steamPath.c_str(), 1);
        } else {
            std::cerr << "Warning: Unable to locate Steam installation at '"
                      << primarySteamPath << "' or '" << fallbackSteamPath << "'." << std::endl;
            std::cerr << "Proton will attempt to auto-discover Steam. "
                      << "If games fail to launch, verify that Steam is installed for this user."
                      << std::endl;
        }
    }

    return true;
}

// ---------------------------------------------------------------------------
// launch
// ---------------------------------------------------------------------------

int WindowsEmulator::launch(const Package& package) {
    if (!m_initialized && !initialize()) {
        std::cerr << "Failed to initialize Windows emulator" << std::endl;
        return 1;
    }

    std::string executablePath = (fs::path(package.getExtractedPath()) / "game" / package.getMainExecutable()).string();

    if (!fs::exists(executablePath)) {
        std::cerr << "Executable not found: " << executablePath << std::endl;
        return 1;
    }

    // Derive a per-title compatdata directory so games don't share a Wine prefix.
    std::string compatDataPath = m_config.getString("proton_data_path", "");
    if (compatDataPath.empty()) {
        const char* home = std::getenv("HOME");
        if (home) {
            std::string compatRoot = std::string(home) + "/.local/share/XEmuRun/compatdata";

            // Use the package name as the directory identifier; sanitize unsafe chars.
            std::string packageId = sanitizeId(package.getName());
            if (packageId.empty()) {
                packageId = sanitizeId(m_config.getString("package_id", ""));
            }
            if (packageId.empty()) {
                packageId = "default";
            }

            compatDataPath = compatRoot + "/" + packageId;
        }
    }

    if (!compatDataPath.empty()) {
        try {
            fs::create_directories(compatDataPath);
        } catch (const fs::filesystem_error& e) {
            std::cerr << "Error: failed to create Proton compatibility data directory '"
                      << compatDataPath << "': " << e.what() << std::endl;
            return 1;
        }
        setenv("STEAM_COMPAT_DATA_PATH", compatDataPath.c_str(), 1);
        std::cout << "Using Proton compatibility data path: " << compatDataPath << std::endl;
    }

    // umu-launcher requires a GAMEID to identify the title; sanitize it
    std::string gameId = m_config.getString("umu_game_id", "");
    if (gameId.empty()) {
        gameId = "umu-" + sanitizeId(package.getName());
    }
    setenv("GAMEID", gameId.c_str(), 1);

    // Launch via umu-run which manages Proton initialization outside Steam.
    // Use fork+execvp to pass the executable path directly as a separate argument,
    // avoiding any shell involvement and the associated shell-injection risk.
    std::cout << "Launching Windows application with Proton-GE via umu-launcher: " << executablePath << std::endl;

    pid_t pid = fork();
    if (pid < 0) {
        std::cerr << "Error: failed to fork process: " << strerror(errno) << std::endl;
        return 1;
    }

    if (pid == 0) {
        // Child: replace process image with umu-run
        char* const argv[] = {
            const_cast<char*>("umu-run"),
            const_cast<char*>(executablePath.c_str()),
            nullptr
        };
        execvp("umu-run", argv);
        // execvp only returns on failure
        std::cerr << "Error: failed to execute umu-run: " << strerror(errno) << std::endl;
        _exit(1);
    }

    // Parent: wait for the child to finish
    int status = 0;
    if (waitpid(pid, &status, 0) == -1) {
        std::cerr << "Error: waitpid failed: " << strerror(errno) << std::endl;
        return 1;
    }

    return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

// ---------------------------------------------------------------------------
// Configuration
// ---------------------------------------------------------------------------

Config WindowsEmulator::getDefaultConfig() const {
    Config config = BaseEmulator::getDefaultConfig();

    // Proton-GE-specific settings
    config.setString("proton_path", "");
    config.setString("proton_data_path", "");
    config.setString("umu_game_id", "");
    config.setInt("windows_version", 10);

    return config;
}

void WindowsEmulator::applyConfig(const Config& config) {
    BaseEmulator::applyConfig(config);

    // Apply Proton-GE-specific settings
    std::string protonPath = config.getString("proton_path", "");
    if (!protonPath.empty()) {
        std::cout << "Using Proton-GE path: " << protonPath << std::endl;
    }

    std::string protonDataPath = config.getString("proton_data_path", "");
    if (!protonDataPath.empty()) {
        std::cout << "Using Proton compatibility data path: " << protonDataPath << std::endl;
    }

    int windowsVersion = config.getInt("windows_version", 10);
    std::cout << "Windows version set to: Windows " << windowsVersion << std::endl;
}

} // namespace XEmuRun
