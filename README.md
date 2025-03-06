# XEmuRun

XEmuRun is a universal game emulation platform that allows users to run games from various platforms (Windows, Linux, PlayStation, Xbox) using a unified interface and package format.

![XEmuRun Logo](resources/icons/app_icon.png)

## Features

- **Universal Game Format (.XEmupkg)** - Package games from any platform into a unified format
- **Multi-Platform Emulation** - Run games from Windows, Linux, PlayStation, and Xbox
- **User-Friendly Interface** - Simple launcher for managing and running your game library
- **Controller Support** - Map controllers for optimal gaming experience
- **Configuration Management** - Platform-specific and game-specific configurations

## Supported Platforms

XEmuRun can run games from:

- Windows (via Wine)
- Linux (native)
- PlayStation 4 & 5 (via emulation)
- Xbox & Xbox Series (via emulation)

## Components

- **XEmuRun CLI** - Command line interface for running games
- **XEmuRun GUI** - Graphical user interface for managing and playing games
- **XEmuPackager CLI** - Command line tool for creating game packages
- **XEmuPackager GUI** - Graphical tool for creating game packages

## Getting Started

See the [Installation Guide](INSTALL.md) for setup instructions and the [Usage Guide](USAGE.md) for information on how to use XEmuRun.

## Requirements

- C++17 compatible compiler (GCC 8+ or Clang 6+)
- CMake 3.10+
- Qt 5.12+
- JsonCpp
- LibArchive
- SDL2 (for controller support)

For automatic installation of dependencies and building:

```bash
chmod +x build.sh
./build.sh
```

## License

XEmuRun is released under the MIT License. See LICENSE file for details.
