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

To run Windows games, Wine must be installed:

```bash
# Ubuntu/Debian
sudo apt install wine

# Fedora
sudo dnf install wine

# Arch Linux
sudo pacman -S wine
```

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

- **Windows Games**: Ensure Wine is properly installed and configured.
- **Controller Issues**: Check that SDL2 is properly installed and your controller is recognized by the system.
