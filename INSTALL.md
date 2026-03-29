# Installation Guide

This guide explains how to install XEmuRun on various platforms.

## Automatic Installation (Ubuntu/Debian)

The easiest way to install XEmuRun on Ubuntu/Debian systems is using the included build script:

```bash
chmod +x build.sh
./build.sh
```

This script will:
1. Install all required dependencies
2. Create a build directory
3. Configure the project with CMake
4. Build all components
5. Install the binaries (optional, requires sudo)

## Manual Installation

### Dependencies

#### Ubuntu/Debian

```bash
sudo apt update
sudo apt install -y build-essential cmake libqt5widgets5 qtbase5-dev libsdl2-dev libarchive-dev libjsoncpp-dev
```

#### Fedora

```bash
sudo dnf install gcc-c++ cmake qt5-qtbase-devel SDL2-devel libarchive-devel jsoncpp-devel
```

#### Arch Linux

```bash
sudo pacman -S base-devel cmake qt5-base sdl2 libarchive jsoncpp
```

### Building from Source

1. Clone the repository:
   ```bash
   git clone https://github.com/yourusername/XEmuRun.git
   cd XEmuRun
   ```

2. Create a build directory:
   ```bash
   mkdir build
   cd build
   ```

3. Configure the project:
   ```bash
   cmake ..
   ```

4. Build:
   ```bash
   cmake --build . -j$(nproc)
   ```

5. Install (optional):
   ```bash
   sudo cmake --install .
   ```

## Platform-Specific Requirements

### Windows Games

Running Windows games requires two components:

#### 1. umu-launcher (required)

`umu-launcher` provides the Steam-compatible runtime layer that Proton needs to
operate outside of Steam. Without it, Proton-GE will not start.

Install via pipx (recommended):
```bash
pipx install umu-launcher
```

Or follow the instructions at: https://github.com/Open-Wine-Components/umu-launcher

#### 2. Proton-GE

1. Download the latest release from:
   ```
   https://github.com/GloriousEggroll/proton-ge-custom/releases
   ```

2. Extract the archive to the Proton compatibility tools directory:
   ```bash
   mkdir -p ~/.local/share/Steam/compatibilitytools.d
   tar -xf GE-Proton*.tar.gz -C ~/.local/share/Steam/compatibilitytools.d/
   ```

XEmuRun will automatically detect the installed Proton-GE version and launch it
via `umu-run`. You can also specify a custom Proton-GE path via the `proton_path`
configuration option.

### PlayStation Emulation

PlayStation emulation requires specific emulator software, which must be installed separately.

### Xbox Emulation

Xbox emulation requires specific emulator software, which must be installed separately.

## Troubleshooting

### Common Issues

#### Missing Dependencies

If you encounter errors about missing dependencies, make sure you've installed all required packages mentioned above.

#### Build Errors

If you experience build errors, check that your compiler supports C++17 and that you have the correct versions of all dependencies.

#### Runtime Errors

- **Windows Games**: Ensure both `umu-launcher` (`umu-run`) and Proton-GE are installed. Proton-GE must be in `~/.local/share/Steam/compatibilitytools.d/` or the path set via the `proton_path` configuration option. See [Windows Games](#windows-games) above for full instructions.
- **Controller Issues**: Check that SDL2 is properly installed and your controller is recognized by the system.
