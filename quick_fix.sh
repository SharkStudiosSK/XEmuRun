#!/bin/bash

# Create the icons directory in the build directory
mkdir -p build/icons

# Create the icons directly in the build directory
convert -size 128x128 xc:#0078D7 -gravity center -pointsize 20 -fill white -annotate 0 "Windows" build/icons/windows.png
convert -size 128x128 xc:#FFA600 -gravity center -pointsize 20 -fill white -annotate 0 "Linux" build/icons/linux.png
convert -size 128x128 xc:#003087 -gravity center -pointsize 20 -fill white -annotate 0 "PlayStation" build/icons/playstation.png
convert -size 128x128 xc:#107C10 -gravity center -pointsize 20 -fill white -annotate 0 "Xbox" build/icons/xbox.png
convert -size 128x128 xc:#4B4B4B -gravity center -pointsize 20 -fill white -annotate 0 "Game" build/icons/game.png
convert -size 128x128 xc:#FF5733 -gravity center -pointsize 20 -fill white -annotate 0 "XEmuRun" build/icons/app_icon.png

echo "Icons created directly in the build/icons directory"
chmod +x quick_fix.sh
