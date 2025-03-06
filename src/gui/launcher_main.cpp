#include <QApplication>
#include <QCommandLineParser>
#include <QMessageBox>
#include <QString>
#include <iostream>
#include "launcher_gui.h"
#include "../config/config_manager.h"

// Forward declaration for direct launch function
bool launchGameDirectly(const QString& packagePath);

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    app.setApplicationName("XEmuRun");
    app.setApplicationDisplayName("XEmuRun Game Launcher");
    app.setApplicationVersion("0.1.0");

    // Set application icon
    app.setWindowIcon(QIcon(":/icons/app_icon.png"));

    // Parse command line arguments
    QCommandLineParser parser;
    parser.setApplicationDescription("Universal Game Emulation Platform");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("package", "XEmuRun package file to open", "[package]");
    
    QCommandLineOption directLaunchOption("direct-launch", "Launch the specified package directly without showing the GUI");
    parser.addOption(directLaunchOption);
    
    parser.process(app);
    
    // Initialize configuration system
    XEmuRun::ConfigManager& configManager = XEmuRun::ConfigManager::getInstance();
    if (!configManager.initialize()) {
        std::cerr << "Failed to initialize configuration system" << std::endl;
        QMessageBox::critical(nullptr, "Error", "Failed to initialize configuration system");
        return 1;
    }
    
    // Check if a package was specified
    const QStringList args = parser.positionalArguments();
    if (!args.isEmpty()) {
        QString packagePath = args.first();
        if (QFile::exists(packagePath)) {
            // If direct launch option is specified, launch the game without showing the GUI
            if (parser.isSet(directLaunchOption)) {
                return launchGameDirectly(packagePath) ? 0 : 1;
            }
            
            // Otherwise, show the GUI and import the package
            XEmuRun::LauncherGui mainWindow;
            mainWindow.show();
            mainWindow.importAndLaunchGame(packagePath);
            return app.exec();
        } else {
            std::cerr << "Package file does not exist: " << packagePath.toStdString() << std::endl;
            return 1;
        }
    }
    
    // No package specified, just show the GUI
    XEmuRun::LauncherGui mainWindow;
    mainWindow.show();
    
    return app.exec();
}

bool launchGameDirectly(const QString& packagePath) {
    try {
        XEmuRun::Launcher launcher;
        if (launcher.loadPackage(packagePath.toStdString())) {
            int result = launcher.runGame();
            return result == 0;
        }
        return false;
    } catch (const std::exception& e) {
        std::cerr << "Error launching game: " << e.what() << std::endl;
        return false;
    }
}
