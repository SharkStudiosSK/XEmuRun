cmake_minimum_required(VERSION 3.10)
project(XEmuRun VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find required packages
find_package(jsoncpp REQUIRED)
find_package(LibArchive REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(SDL2 REQUIRED)

# Define source files
set(SOURCES
    src/main.cpp
    src/launcher/launcher.cpp
    src/package/package.cpp
    src/packager/packager.cpp
    src/config/config.cpp
    src/config/config_manager.cpp
    src/utils/archive.cpp
)

# Add emulator implementations
file(GLOB EMULATOR_SOURCES src/emulators/*.cpp)
list(APPEND SOURCES ${EMULATOR_SOURCES})

# Create resources file for icons
configure_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/resources/resources.qrc
    ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc
    COPYONLY
)

# Make sure the icons are copied to the correct location in the build directory
file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/icons)
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/resources/icons/
     DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/icons)

# XEmuRun CLI executable
add_executable(xemurun ${SOURCES})
target_include_directories(xemurun PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(xemurun PRIVATE 
    JsonCpp::JsonCpp
    ${LibArchive_LIBRARIES}
)
target_include_directories(xemurun PRIVATE ${LibArchive_INCLUDE_DIRS})

# XEmuPackager CLI tool
add_executable(xemupackager 
    src/packager/main.cpp 
    src/packager/packager.cpp 
    src/utils/archive.cpp 
    src/config/config.cpp
    src/config/config_manager.cpp
)
target_include_directories(xemupackager PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(xemupackager PRIVATE 
    JsonCpp::JsonCpp
    ${LibArchive_LIBRARIES}
)
target_include_directories(xemupackager PRIVATE ${LibArchive_INCLUDE_DIRS})

# XEmuPackager GUI tool
add_executable(xemupackager-gui
    src/gui/main_gui.cpp
    src/gui/packager_gui.cpp
    src/packager/packager.cpp
    src/utils/archive.cpp
    src/config/config.cpp
    src/config/config_manager.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc
)
target_include_directories(xemupackager-gui PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_link_libraries(xemupackager-gui PRIVATE
    JsonCpp::JsonCpp
    ${LibArchive_LIBRARIES}
    Qt5::Widgets
)
target_include_directories(xemupackager-gui PRIVATE ${LibArchive_INCLUDE_DIRS})

# XEmuRun GUI launcher
add_executable(xemurun-gui
    src/gui/launcher_main.cpp
    src/gui/launcher_gui.cpp
    src/gui/game_library.cpp
    src/gui/controller_mapping.cpp
    src/launcher/launcher.cpp
    src/package/package.cpp
    src/config/config.cpp
    src/config/config_manager.cpp
    src/utils/archive.cpp
    ${CMAKE_CURRENT_BINARY_DIR}/resources.qrc
    ${EMULATOR_SOURCES}  # Add all emulator sources
)

target_include_directories(xemurun-gui PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${LibArchive_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
)

target_link_libraries(xemurun-gui PRIVATE
    JsonCpp::JsonCpp
    ${LibArchive_LIBRARIES}
    Qt5::Widgets
    ${SDL2_LIBRARIES}
)

# Install
install(TARGETS xemurun xemupackager xemupackager-gui xemurun-gui DESTINATION bin)