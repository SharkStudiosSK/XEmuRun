#!/bin/bash

# Create icons directory if it doesn't exist
mkdir -p resources/icons

# Function to create a simple colored square icon with text
create_icon() {
    local name=$1
    local color=$2
    local text=$3
    local size=128

    # Check if ImageMagick is installed
    if ! command -v convert &> /dev/null; then
        echo "Error: ImageMagick is not installed."
        echo "Please install it using: sudo apt-get install imagemagick"
        exit 1
    fi

    # Create a colored square with text
    convert -size ${size}x${size} xc:${color} \
        -gravity center -pointsize 20 -fill white \
        -annotate 0 "${text}" \
        resources/icons/${name}.png

    echo "Created icon: resources/icons/${name}.png"
}

# Create icons with different colors
create_icon "windows" "#0078D7" "Windows"
create_icon "linux" "#FFA600" "Linux"
create_icon "playstation" "#003087" "PlayStation"
create_icon "xbox" "#107C10" "Xbox"
create_icon "game" "#4B4B4B" "Game"
create_icon "app_icon" "#FF5733" "XEmuRun"

echo "All icons created successfully!"
