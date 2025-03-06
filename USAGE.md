# XEmuRun Usage Guide

This guide explains how to use XEmuRun to package and run games from various platforms.

## Creating Game Packages

### Using the GUI Packager

1. Launch the XEmuRun Packager GUI:
   ```bash
   xemupackager-gui
   ```

2. In the interface:
   - Select the path to the game files
   - Choose an output directory for the package
   - Enter the game name
   - Select the platform (Windows, Linux, PlayStation, Xbox)
   - Specify the main executable (relative to the game directory)
   - Add any platform-specific configuration options
   - Click "Create Package"

3. The package will be created in the specified output directory with a `.XEmupkg` extension.

### Using the Command Line Packager

1. Use the `xemupackager` command with appropriate arguments:
   ```bash
   xemupackager --game-path /path/to/game \
                --output-path /output/directory \
                --name "Game Name" \
                --platform windows \
                --main "executable.exe" \
                --config fullscreen=true \
                --config resolution_width=1920 \
                --config resolution_height=1080
   ```

2. For automatic platform detection:
   ```bash
   xemupackager --game-path /path/to/game \
                --output-path /output/directory \
                --name "Game Name" \
                --auto-detect
   ```

## Running Games

### Using the GUI Launcher

1. Launch the XEmuRun GUI:
   ```bash
   xemurun-gui
   ```

2. In the Game Library tab:
   - Click "Import Game" to add a `.XEmupkg` file to your library
   - Select a game from the list
   - Click "Launch Game" to start playing

3. To scan a directory for multiple packages:
   - Click "File" > "Scan for Games..."
   - Select a directory containing `.XEmupkg` files

### Using the Command Line Launcher

1. Run a game package directly:
   ```bash
   xemurun /path/to/game.XEmupkg
   ```

## Controller Configuration

1. In the XEmuRun GUI, go to the "Controllers" tab.
2. Select your controller from the dropdown list.
3. Choose the platform you're configuring for (e.g., Windows, PlayStation).
4. Click "Start Mapping" to begin the button mapping process.
5. Press the buttons on your controller when prompted.
6. Click "Save Mapping" to store your configuration.

## Configuration Settings

### Global Settings

Global settings can be configured in the "Settings" tab of the XEmuRun GUI:
- Theme selection
- Emulator paths
- Default directories

### Platform-Specific Settings

Each platform has its own configuration options:

#### Windows (Wine)
- Wine prefix location
- Windows version (7, 8, 10)
- DXVK support
- Resolution and fullscreen mode

#### PlayStation
- BIOS path
- Rendering resolution
- Controller settings

#### Xbox
- System files path
- Hardware acceleration options

### Game-Specific Settings

Game-specific settings are stored in the `.XEmupkg` file and can be set during the packaging process.

## Command Line Reference

### XEmuRun Launcher

```
xemurun [path_to_xemupkg]
```

### XEmuRun GUI Launcher

```
xemurun-gui [options] [path_to_xemupkg]

Options:
  --direct-launch    Launch the specified package directly without showing the GUI
```

### XEmuPackager

```
xemupackager [options]

Options:
  --game-path <path>       Path to the game files
  --output-path <path>     Where to save the .XEmupkg file
  --name <game_name>       Name of the game
  --platform <platform>    Platform (windows, linux, playstation4, etc.)
  --main <executable>      Main executable path (relative to game directory)
  --config key=value       Add configuration key-value pair
  --auto-detect            Try to automatically detect platform
  --gui                    Launch the graphical interface
  --help, -h               Display help message
```
