#include <QApplication>
#include "packager_gui.h"
#include "../config/config_manager.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("XEmuRun Package Creator");
    
    // Initialize configuration system
    XEmuRun::ConfigManager& configManager = XEmuRun::ConfigManager::getInstance();
    if (!configManager.initialize()) {
        std::cerr << "Failed to initialize configuration system" << std::endl;
        return 1;
    }
    
    XEmuRun::PackagerGui mainWindow;
    mainWindow.show();
    
    return app.exec();
}
