#!/bin/bash
# filepath: /home/misal/Plocha/Projects/XEmuRun/flatpak/publish_to_flathub.sh

set -e  # Exit immediately if a command exits with a non-zero status

# Colors for output
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
RED='\033[0;31m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

FLATHUB_REPO="https://github.com/flathub"
APP_ID="io.github.xemurun"
APP_NAME="XEmuRun"
GITHUB_USERNAME=""
FLATPAK_DIR="$(pwd)/flatpak"
MANIFEST_FILE="${APP_ID}.json"

# Function to check dependencies
check_dependencies() {
    echo -e "${BLUE}Checking dependencies...${NC}"
    
    local MISSING_DEPS=0
    for cmd in git flatpak flatpak-builder jq; do
        if ! command -v $cmd &> /dev/null; then
            echo -e "${RED}Missing dependency: $cmd${NC}"
            MISSING_DEPS=1
        fi
    done
    
    if [ $MISSING_DEPS -eq 1 ]; then
        echo -e "${RED}Please install missing dependencies.${NC}"
        echo "For Ubuntu/Debian: sudo apt install git flatpak flatpak-builder jq"
        echo "For Fedora: sudo dnf install git flatpak flatpak-builder jq"
        exit 1
    fi
    
    # Check if Flathub repo is added
    if ! flatpak remote-list | grep -q "flathub"; then
        echo -e "${YELLOW}Adding Flathub repository...${NC}"
        flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
    fi
    
    echo -e "${GREEN}All dependencies are installed!${NC}"
}

# Function to create the Flatpak manifest
create_manifest() {
    echo -e "${BLUE}Creating Flatpak manifest...${NC}"
    
    mkdir -p "$FLATPAK_DIR"
    
    cat > "$FLATPAK_DIR/$MANIFEST_FILE" << EOL
{
    "app-id": "${APP_ID}",
    "runtime": "org.kde.Platform",
    "runtime-version": "5.15-22.08",
    "sdk": "org.kde.Sdk",
    "command": "xemurun-gui",
    "finish-args": [
        "--share=ipc",
        "--socket=x11",
        "--socket=wayland",
        "--socket=pulseaudio",
        "--device=dri",
        "--filesystem=home",
        "--talk-name=org.freedesktop.Notifications"
    ],
    "modules": [
        {
            "name": "jsoncpp",
            "buildsystem": "cmake-ninja",
            "sources": [
                {
                    "type": "git",
                    "url": "https://github.com/open-source-parsers/jsoncpp.git",
                    "tag": "1.9.5"
                }
            ]
        },
        {
            "name": "SDL2",
            "buildsystem": "autotools",
            "sources": [
                {
                    "type": "archive",
                    "url": "https://www.libsdl.org/release/SDL2-2.0.22.tar.gz",
                    "sha256": "fe7cbf3127882e3fc7259a75a0cb585620272c51745d3852ab9dd87960697f2e"
                }
            ]
        },
        {
            "name": "libarchive",
            "buildsystem": "autotools",
            "sources": [
                {
                    "type": "archive",
                    "url": "https://www.libarchive.org/downloads/libarchive-3.6.2.tar.gz",
                    "sha256": "9e2c1b80d5fbe59b61308fdfab6c79b5021d7ff4ff2489fb12daf0a96a83551d"
                }
            ]
        },
        {
            "name": "xemurun",
            "buildsystem": "cmake-ninja",
            "config-opts": [
                "-DCMAKE_BUILD_TYPE=RelWithDebInfo"
            ],
            "sources": [
                {
                    "type": "dir",
                    "path": ".."
                }
            ]
        }
    ]
}
EOL
    
    echo -e "${GREEN}Created manifest at $FLATPAK_DIR/$MANIFEST_FILE${NC}"
}

# Function to create the metainfo file
create_metainfo() {
    echo -e "${BLUE}Creating metainfo file...${NC}"
    
    mkdir -p "$FLATPAK_DIR/metadata"
    
    cat > "$FLATPAK_DIR/metadata/${APP_ID}.metainfo.xml" << EOL
<?xml version="1.0" encoding="UTF-8"?>
<component type="desktop-application">
  <id>${APP_ID}</id>
  <name>XEmuRun</name>
  <summary>Universal Game Emulation Platform</summary>
  <description>
    <p>
      XEmuRun allows you to run games from various platforms (Windows, Linux, PlayStation, Xbox) 
      using a unified interface and package format.
    </p>
    <p>Features:</p>
    <ul>
      <li>Universal Game Format (.XEmupkg) - Package games from any platform into a unified format</li>
      <li>Multi-Platform Emulation - Run games from Windows, Linux, PlayStation, and Xbox</li>
      <li>User-Friendly Interface - Simple launcher for managing and running your game library</li>
      <li>Controller Support - Map controllers for optimal gaming experience</li>
      <li>Configuration Management - Platform-specific and game-specific configurations</li>
    </ul>
  </description>
  <url type="homepage">https://github.com/yourname/XEmuRun</url>
  <developer_name>XEmuRun Project</developer_name>
  <content_rating type="oars-1.1" />
  <launchable type="desktop-id">${APP_ID}.desktop</launchable>
  <screenshots>
    <screenshot type="default">
      <caption>Main application window</caption>
      <image>https://example.com/screenshots/xemurun-screenshot1.png</image>
    </screenshot>
  </screenshots>
  <releases>
    <release version="0.1.0" date="$(date +%Y-%m-%d)">
      <description>
        <p>Initial release of XEmuRun.</p>
      </description>
    </release>
  </releases>
</component>
EOL

    echo -e "${GREEN}Created metainfo file at $FLATPAK_DIR/metadata/${APP_ID}.metainfo.xml${NC}"
}

# Function to create the desktop file
create_desktop_file() {
    echo -e "${BLUE}Creating desktop file...${NC}"
    
    mkdir -p "$FLATPAK_DIR/metadata"
    
    cat > "$FLATPAK_DIR/metadata/${APP_ID}.desktop" << EOL
[Desktop Entry]
Name=XEmuRun
Comment=Universal Game Emulation Platform
Exec=xemurun-gui
Icon=${APP_ID}
Terminal=false
Type=Application
Categories=Game;Emulator;Utility;
Keywords=emulator;game;gaming;
StartupWMClass=XEmuRun
EOL

    echo -e "${GREEN}Created desktop file at $FLATPAK_DIR/metadata/${APP_ID}.desktop${NC}"
}

# Function to build the Flatpak
build_flatpak() {
    echo -e "${BLUE}Building Flatpak...${NC}"
    
    cd "$FLATPAK_DIR"
    
    # Install SDK if not already installed
    if ! flatpak list --runtime | grep -q "org.kde.Sdk//5.15-22.08"; then
        echo "Installing KDE SDK 5.15-22.08..."
        flatpak install -y flathub org.kde.Sdk//5.15-22.08
        flatpak install -y flathub org.kde.Platform//5.15-22.08
    fi
    
    # Build the application
    echo "Building application... This may take a while."
    flatpak-builder --force-clean build "$MANIFEST_FILE"
    
    # Create a repository
    echo "Creating local repository..."
    flatpak-builder --repo=repo --force-clean build "$MANIFEST_FILE"
    
    # Test the build
    echo "Testing build..."
    flatpak-builder --run build "$MANIFEST_FILE" xemurun-gui --version
    
    echo -e "${GREEN}Flatpak built successfully!${NC}"
}

# Function to guide through the Flathub submission process
submit_to_flathub() {
    echo -e "${BLUE}Preparing for Flathub submission...${NC}"
    
    # Ask for GitHub username
    read -p "Enter your GitHub username: " GITHUB_USERNAME
    
    if [ -z "$GITHUB_USERNAME" ]; then
        echo -e "${RED}GitHub username is required for Flathub submission.${NC}"
        exit 1
    fi

    echo ""
    echo -e "${YELLOW}To submit to Flathub, follow these steps:${NC}"
    echo "1. Fork the flathub repository: https://github.com/flathub/flathub"
    echo "2. Create a new branch in your fork"
    echo "3. Create a new directory in your fork named after your app ID: ${APP_ID}"
    echo "4. Copy these files to that directory:"
    echo "   - ${MANIFEST_FILE}"
    echo "   - metadata/${APP_ID}.metainfo.xml"
    echo "   - metadata/${APP_ID}.desktop"
    echo "5. Commit and push your changes to your fork"
    echo "6. Create a pull request to the flathub repository"
    echo ""
    
    # Create a GitHub repository with the correct files
    echo -e "${YELLOW}Would you like to create a GitHub repository with the correct structure now? (y/n)${NC}"
    read -r answer
    
    if [[ "$answer" == "y" || "$answer" == "Y" ]]; then
        REPO_DIR="$FLATPAK_DIR/flathub-${APP_ID}"
        
        echo "Creating repository structure..."
        mkdir -p "$REPO_DIR/${APP_ID}"
        cp "$FLATPAK_DIR/$MANIFEST_FILE" "$REPO_DIR/${APP_ID}/"
        mkdir -p "$REPO_DIR/${APP_ID}/metadata"
        cp "$FLATPAK_DIR/metadata/${APP_ID}.metainfo.xml" "$REPO_DIR/${APP_ID}/metadata/"
        cp "$FLATPAK_DIR/metadata/${APP_ID}.desktop" "$REPO_DIR/${APP_ID}/metadata/"
        
        # Create README for the repo
        cat > "$REPO_DIR/README.md" << EOL
# ${APP_NAME} for Flathub

This repository contains the Flatpak manifest for ${APP_NAME}.

## Building

To build and install the Flatpak locally:

\`\`\`bash
flatpak-builder --user --install --force-clean build-dir ${APP_ID}/${APP_ID}.json
\`\`\`

## Running

\`\`\`bash
flatpak run ${APP_ID}
\`\`\`
EOL
        
        echo -e "${GREEN}Repository structure created at: $REPO_DIR${NC}"
        echo "You can now initialize a Git repository here and push to GitHub."
        echo ""
        echo "Initialize and push to GitHub with:"
        echo "  cd $REPO_DIR"
        echo "  git init"
        echo "  git add ."
        echo "  git commit -m \"Add ${APP_NAME} Flatpak manifest\""
        echo "  gh repo create ${GITHUB_USERNAME}/flathub-${APP_ID} --public --source=. --push"
        echo ""
        echo "Then create a pull request to Flathub by visiting:"
        echo "  https://github.com/flathub/flathub/fork"
    fi
    
    echo -e "${YELLOW}For detailed instructions on submitting to Flathub, visit:${NC}"
    echo "https://docs.flathub.org/docs/for-app-authors/submission"
}

# Main function
main() {
    echo -e "${GREEN}XEmuRun Flathub Publishing Tool${NC}"
    echo "This script will prepare the XEmuRun application for Flathub submission."
    echo ""
    
    check_dependencies
    create_manifest
    create_metainfo
    create_desktop_file
    build_flatpak
    submit_to_flathub
    
    echo -e "${GREEN}All done!${NC}"
    echo "Your application is now ready for Flathub submission."
}

main "$@"