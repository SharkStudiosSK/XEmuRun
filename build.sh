#!/bin/bash
# filepath: /home/misal/Plocha/Projects/XEmuRun/build.sh

set -e  # Exit immediately if a command exits with a non-zero status

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
NC='\033[0m' # No Color

echo -e "${GREEN}XEmuRun Build Script${NC}"
echo "This script will install required dependencies and build XEmuRun."
echo ""

# Function to check if running as root
check_root() {
    if [ "$EUID" -eq 0 ]; then
        echo -e "${RED}Please don't run this script as root or with sudo.${NC}"
        echo "The script will ask for sudo permissions when necessary."
        exit 1
    fi
}

# Function to check if a command exists
command_exists() {
    command -v "$1" &> /dev/null
}

# Function to install dependencies on Debian/Ubuntu
install_dependencies_debian() {
    echo -e "${GREEN}Installing dependencies...${NC}"
    
    sudo apt update
    sudo apt install -y build-essential cmake \
        qtbase5-dev libqt5widgets5 \
        libsdl2-dev libarchive-dev libjsoncpp-dev \
        wine-stable imagemagick
        
    echo -e "${GREEN}Dependencies installed successfully!${NC}"
}

# Function to detect the Linux distribution
detect_distro() {
    if command_exists apt-get; then
        echo "debian"
    elif command_exists dnf; then
        echo "fedora"
    elif command_exists pacman; then
        echo "arch"
    else
        echo "unknown"
    fi
}

# Create icon directories and placeholder icons
create_icons() {
    echo "Checking for icon files..."
    
    mkdir -p resources/icons
    
    # Check if any of the required icons are missing
    if [ ! -f resources/icons/app_icon.png ] || 
       [ ! -f resources/icons/windows.png ] ||
       [ ! -f resources/icons/linux.png ] ||
       [ ! -f resources/icons/playstation.png ] ||
       [ ! -f resources/icons/xbox.png ] ||
       [ ! -f resources/icons/game.png ]; then
        
        echo "Some icon files are missing. Creating placeholder icons..."
        
        # Check if ImageMagick is installed
        if command_exists convert; then
            # Use create_icons.sh to generate placeholder icons
            chmod +x create_icons.sh
            ./create_icons.sh
        else
            echo -e "${YELLOW}Warning: ImageMagick not found. Cannot create placeholder icons.${NC}"
            echo "Please install ImageMagick (sudo apt install imagemagick) or"
            echo "manually add icon files to resources/icons/ directory:"
            echo "  - app_icon.png"
            echo "  - windows.png"
            echo "  - linux.png"
            echo "  - playstation.png"
            echo "  - xbox.png"
            echo "  - game.png"
        fi
    else
        echo "All required icon files found."
    fi
}

# Main build function
build_xemurun() {
    echo -e "${GREEN}Building XEmuRun...${NC}"
    
    # Create build directory
    mkdir -p build
    cd build
    
    # Configure
    echo "Configuring with CMake..."
    cmake ..
    
    # Build
    echo "Compiling..."
    cmake --build . -j$(nproc)
    
    echo -e "${GREEN}Build completed successfully!${NC}"
}

# Install function
install_xemurun() {
    echo -e "${YELLOW}Do you want to install XEmuRun system-wide? (y/n)${NC}"
    read -r answer
    
    if [[ "$answer" == "y" || "$answer" == "Y" ]]; then
        echo "Installing..."
        sudo cmake --install .
        echo -e "${GREEN}Installation completed!${NC}"
    else
        echo -e "${YELLOW}Skipping installation.${NC}"
        echo "The binaries are available in the build directory:"
        echo "  - XEmuRun CLI: $(pwd)/xemurun"
        echo "  - XEmuRun GUI: $(pwd)/xemurun-gui"
        echo "  - XEmuPackager CLI: $(pwd)/xemupackager"
        echo "  - XEmuPackager GUI: $(pwd)/xemupackager-gui"
    fi
}

# Main execution
check_root

# Detect distribution and install dependencies
distro=$(detect_distro)
case $distro in
    debian)
        install_dependencies_debian
        ;;
    fedora)
        echo -e "${RED}Fedora is not yet supported by this script.${NC}"
        echo "Please install dependencies manually as described in INSTALL.md"
        exit 1
        ;;
    arch)
        echo -e "${RED}Arch Linux is not yet supported by this script.${NC}"
        echo "Please install dependencies manually as described in INSTALL.md"
        exit 1
        ;;
    *)
        echo -e "${RED}Unable to detect your distribution.${NC}"
        echo "Please install dependencies manually as described in INSTALL.md"
        exit 1
        ;;
esac

# Create icons
create_icons

# Build the project
build_xemurun

# Install if requested
install_xemurun

echo -e "${GREEN}All done!${NC}"
echo "You can now use XEmuRun to package and run games."
echo "See USAGE.md for instructions on how to use XEmuRun."

exit 0