#include "launcher_gui.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QDesktopServices>
#include <QUrl>
#include <QFileInfo>
#include <QDirIterator>
#include <QPixmap>
#include <QFormLayout>  // Add this for QFormLayout
#include <QApplication> // Add this for qApp
#include "../config/config_manager.h"

namespace XEmuRun {

LauncherGui::LauncherGui(QWidget *parent)
    : QMainWindow(parent), m_launcher(std::make_unique<Launcher>()) {
    setWindowTitle("XEmuRun Game Launcher");
    setMinimumSize(800, 600);
    
    setupUi();
    setupMenus();
    connectSignals();
    loadSettings();
    
    // Populate initial game library
    refreshLibrary();
    
    statusBar()->showMessage("Ready");
}

LauncherGui::~LauncherGui() {
    saveSettings();
}

void LauncherGui::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Create tab widget
    m_tabWidget = new QTabWidget(centralWidget);
    
    // Create library tab
    m_libraryTab = new QWidget();
    QHBoxLayout* libraryLayout = new QHBoxLayout(m_libraryTab);
    
    // Left side - game list
    QVBoxLayout* leftLayout = new QVBoxLayout();
    m_gamesList = new QListWidget();
    m_gamesList->setIconSize(QSize(48, 48));
    m_gamesList->setMinimumWidth(300);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_importButton = new QPushButton("Import Game");
    m_launchButton = new QPushButton("Launch Game");
    m_refreshButton = new QPushButton("Refresh");
    m_launchButton->setEnabled(false);
    
    buttonLayout->addWidget(m_importButton);
    buttonLayout->addWidget(m_launchButton);
    buttonLayout->addWidget(m_refreshButton);
    
    leftLayout->addWidget(m_gamesList);
    leftLayout->addLayout(buttonLayout);
    
    // Right side - game info
    QVBoxLayout* rightLayout = new QVBoxLayout();
    m_gameIconLabel = new QLabel();
    m_gameIconLabel->setMinimumSize(128, 128);
    m_gameIconLabel->setAlignment(Qt::AlignCenter);
    
    m_gameInfoLabel = new QLabel("Select a game to view details");
    m_gameInfoLabel->setWordWrap(true);
    m_gameInfoLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    
    m_gamePlatformLabel = new QLabel();
    
    rightLayout->addWidget(m_gameIconLabel);
    rightLayout->addWidget(m_gamePlatformLabel);
    rightLayout->addWidget(m_gameInfoLabel, 1);
    rightLayout->addStretch();
    
    // Add to library layout
    libraryLayout->addLayout(leftLayout);
    libraryLayout->addLayout(rightLayout);
    
    // Create controllers tab
    m_controllersTab = new QWidget();
    QVBoxLayout* controllersLayout = new QVBoxLayout(m_controllersTab);
    m_controllerMapping = new ControllerMapping(m_controllersTab);
    controllersLayout->addWidget(m_controllerMapping);
    
    // Create settings tab
    m_settingsTab = new QWidget();
    QVBoxLayout* settingsLayout = new QVBoxLayout(m_settingsTab);
    
    // General settings
    m_generalSettingsGroup = new QGroupBox("General Settings");
    QFormLayout* generalSettingsLayout = new QFormLayout(m_generalSettingsGroup);
    m_themeCombo = new QComboBox();
    m_themeCombo->addItem("Default");
    m_themeCombo->addItem("Dark");
    m_themeCombo->addItem("Light");
    generalSettingsLayout->addRow("Theme:", m_themeCombo);
    
    // Emulator paths
    m_emulatorPathsGroup = new QGroupBox("Emulator Paths");
    QVBoxLayout* emulatorPathsLayout = new QVBoxLayout(m_emulatorPathsGroup);
    m_emulatorPathsView = new QTreeView();
    m_emulatorPathsModel = new QStandardItemModel(0, 2);
    m_emulatorPathsModel->setHeaderData(0, Qt::Horizontal, "Emulator");
    m_emulatorPathsModel->setHeaderData(1, Qt::Horizontal, "Path");
    m_emulatorPathsView->setModel(m_emulatorPathsModel);
    m_emulatorPathsView->setAlternatingRowColors(true);
    m_emulatorPathsView->setRootIsDecorated(false);
    emulatorPathsLayout->addWidget(m_emulatorPathsView);
    
    populatePlatforms();
    
    // Apply button
    m_applySettingsButton = new QPushButton("Apply Settings");
    
    // Add to settings layout
    settingsLayout->addWidget(m_generalSettingsGroup);
    settingsLayout->addWidget(m_emulatorPathsGroup);
    settingsLayout->addWidget(m_applySettingsButton);
    settingsLayout->addStretch();
    
    // Add tabs to tab widget
    m_tabWidget->addTab(m_libraryTab, "Game Library");
    m_tabWidget->addTab(m_controllersTab, "Controllers");
    m_tabWidget->addTab(m_settingsTab, "Settings");
    
    // Add tab widget to main layout
    mainLayout->addWidget(m_tabWidget);
    
    // Create game library handler
    m_gameLibrary = new GameLibrary(this);
    
    // Setup status bar
    m_statusLabel = new QLabel("Ready");
    statusBar()->addPermanentWidget(m_statusLabel);
}

void LauncherGui::setupMenus() {
    // Create menu bar
    QMenu* fileMenu = menuBar()->addMenu("File");
    QAction* importAction = fileMenu->addAction("Import Game...");
    connect(importAction, &QAction::triggered, this, &LauncherGui::importGame);
    
    fileMenu->addSeparator();
    QAction* scanAction = fileMenu->addAction("Scan for Games...");
    connect(scanAction, &QAction::triggered, this, &LauncherGui::scanForGames);
    
    fileMenu->addSeparator();
    QAction* exitAction = fileMenu->addAction("Exit");
    connect(exitAction, &QAction::triggered, this, &QWidget::close);
    
    QMenu* settingsMenu = menuBar()->addMenu("Settings");
    QAction* settingsAction = settingsMenu->addAction("Preferences...");
    connect(settingsAction, &QAction::triggered, this, &LauncherGui::openSettings);
    
    QMenu* helpMenu = menuBar()->addMenu("Help");
    QAction* aboutAction = helpMenu->addAction("About XEmuRun...");
    connect(aboutAction, &QAction::triggered, this, &LauncherGui::showAboutDialog);
}

void LauncherGui::connectSignals() {
    connect(m_importButton, &QPushButton::clicked, this, &LauncherGui::importGame);
    connect(m_launchButton, &QPushButton::clicked, this, &LauncherGui::launchGame);
    connect(m_refreshButton, &QPushButton::clicked, this, &LauncherGui::refreshLibrary);
    connect(m_gamesList, &QListWidget::itemClicked, this, &LauncherGui::gameSelected);
    connect(m_gamesList, &QListWidget::itemDoubleClicked, this, [this](QListWidgetItem*) {
        launchGame();
    });
    connect(m_applySettingsButton, &QPushButton::clicked, this, &LauncherGui::applySettings);
}

void LauncherGui::loadSettings() {
    QSettings settings("XEmuRun", "Launcher");
    
    // Restore window geometry
    if (settings.contains("geometry")) {
        restoreGeometry(settings.value("geometry").toByteArray());
    }
    
    // Load theme
    if (settings.contains("theme")) {
        m_themeCombo->setCurrentText(settings.value("theme").toString());
    }
    
    // Load emulator paths from ConfigManager
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        Config& systemConfig = configManager.getSystemConfig();
        
        // Apply theme
        QString theme = m_themeCombo->currentText();
        applyTheme(theme);
    }
}

void LauncherGui::saveSettings() {
    QSettings settings("XEmuRun", "Launcher");
    
    // Save window geometry
    settings.setValue("geometry", saveGeometry());
    
    // Save theme
    settings.setValue("theme", m_themeCombo->currentText());
    
    // Save emulator paths to ConfigManager
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        Config& systemConfig = configManager.getSystemConfig();
        
        for (int row = 0; row < m_emulatorPathsModel->rowCount(); ++row) {
            QString platform = m_emulatorPathsModel->data(m_emulatorPathsModel->index(row, 0)).toString();
            QString path = m_emulatorPathsModel->data(m_emulatorPathsModel->index(row, 1)).toString();
            
            systemConfig.setString("emulator_path_" + platform.toStdString(), path.toStdString());
        }
        
        configManager.saveSystemConfig();
    }
}

void LauncherGui::populatePlatforms() {
    m_emulatorPathsModel->setRowCount(0);
    
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        Config& systemConfig = configManager.getSystemConfig();
        
        std::vector<std::string> platforms = {
            "windows", "linux", "playstation4", "playstation5", "xbox", "xbox_series"
        };
        
        for (const auto& platform : platforms) {
            QString platformName = QString::fromStdString(platform);
            QString path = QString::fromStdString(systemConfig.getString("emulator_path_" + platform, ""));
            
            QList<QStandardItem*> row;
            row << new QStandardItem(platformName);
            row << new QStandardItem(path);
            m_emulatorPathsModel->appendRow(row);
        }
    }
}

void LauncherGui::importGame() {
    QString filePath = QFileDialog::getOpenFileName(this, "Import Game Package", "", "XEmuRun Packages (*.XEmupkg)");
    
    if (filePath.isEmpty()) {
        return; // User canceled
    }
    
    QFileInfo fileInfo(filePath);
    if (!fileInfo.exists() || fileInfo.suffix().toLower() != "xemupkg") {
        QMessageBox::warning(this, "Import Error", "Selected file is not a valid XEmuRun package.");
        return;
    }
    
    // Add to library
    if (m_gameLibrary->addGame(filePath)) {
        refreshLibrary();
        statusBar()->showMessage("Game imported successfully", 3000);
    } else {
        QMessageBox::warning(this, "Import Error", "Failed to import the game package.");
    }
}

void LauncherGui::launchGame() {
    QListWidgetItem* currentItem = m_gamesList->currentItem();
    if (!currentItem) {
        QMessageBox::information(this, "Launch Game", "Please select a game to launch.");
        return;
    }
    
    QString packagePath = currentItem->data(Qt::UserRole).toString();
    if (packagePath.isEmpty()) {
        QMessageBox::warning(this, "Launch Error", "Invalid game package.");
        return;
    }
    
    statusBar()->showMessage("Launching game...", 2000);
    
    try {
        if (m_launcher->loadPackage(packagePath.toStdString())) {
            int result = m_launcher->runGame();
            
            if (result != 0) {
                QMessageBox::warning(this, "Launch Error", 
                                    "The game exited with an error code: " + QString::number(result));
            }
        } else {
            QMessageBox::warning(this, "Launch Error", "Failed to load the game package.");
        }
    } catch (const std::exception& e) {
        QMessageBox::critical(this, "Error", QString("An error occurred: %1").arg(e.what()));
    }
}

void LauncherGui::refreshLibrary() {
    m_gamesList->clear();
    m_gameInfoLabel->setText("Select a game to view details");
    m_gameIconLabel->clear();
    m_gamePlatformLabel->clear();
    
    const QList<GameInfo> games = m_gameLibrary->getGames();
    
    for (const auto& game : games) {
        QListWidgetItem* item = new QListWidgetItem(game.name);
        item->setData(Qt::UserRole, game.packagePath);
        
        // Set icon if available
        if (!game.iconPath.isEmpty() && QFile::exists(game.iconPath)) {
            item->setIcon(QIcon(game.iconPath));
        } else {
            // Default icon based on platform
            if (game.platform == "windows") {
                item->setIcon(QIcon(":/icons/windows.png"));
            } else if (game.platform == "linux") {
                item->setIcon(QIcon(":/icons/linux.png"));
            } else if (game.platform.startsWith("playstation")) {
                item->setIcon(QIcon(":/icons/playstation.png"));
            } else if (game.platform.startsWith("xbox")) {
                item->setIcon(QIcon(":/icons/xbox.png"));
            } else {
                item->setIcon(QIcon(":/icons/game.png"));
            }
        }
        
        m_gamesList->addItem(item);
    }
    
    statusBar()->showMessage(QString("Loaded %1 games").arg(games.size()), 3000);
}

void LauncherGui::gameSelected(QListWidgetItem* item) {
    if (!item) return;
    
    QString packagePath = item->data(Qt::UserRole).toString();
    if (packagePath.isEmpty()) return;
    
    m_launchButton->setEnabled(true);
    
    GameInfo game = m_gameLibrary->getGameInfo(packagePath);
    showGameDetails(game);
}

void LauncherGui::showGameDetails(const GameInfo& game) {
    // Display game icon
    if (!game.iconPath.isEmpty() && QFile::exists(game.iconPath)) {
        QPixmap pixmap(game.iconPath);
        m_gameIconLabel->setPixmap(pixmap.scaled(128, 128, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        m_gameIconLabel->clear();
    }
    
    // Display platform
    m_gamePlatformLabel->setText(QString("Platform: %1").arg(game.platform));
    
    // Display detailed info
    QString info = QString("<h2>%1</h2>").arg(game.name);
    info += QString("<p><b>Version:</b> %1</p>").arg(game.version);
    
    if (!game.description.isEmpty()) {
        info += QString("<p>%1</p>").arg(game.description);
    }
    
    m_gameInfoLabel->setText(info);
}

void LauncherGui::applySettings() {
    // Save and apply settings
    saveSettings();
    
    // Apply theme
    QString theme = m_themeCombo->currentText();
    applyTheme(theme);
    
    QMessageBox::information(this, "Settings", "Settings applied successfully.");
}

void LauncherGui::applyTheme(const QString& theme) {
    QPalette palette;
    
    if (theme == "Dark") {
        palette.setColor(QPalette::Window, QColor(53, 53, 53));
        palette.setColor(QPalette::WindowText, Qt::white);
        palette.setColor(QPalette::Base, QColor(25, 25, 25));
        palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
        palette.setColor(QPalette::ToolTipBase, Qt::white);
        palette.setColor(QPalette::ToolTipText, Qt::white);
        palette.setColor(QPalette::Text, Qt::white);
        palette.setColor(QPalette::Button, QColor(53, 53, 53));
        palette.setColor(QPalette::ButtonText, Qt::white);
        palette.setColor(QPalette::BrightText, Qt::red);
        palette.setColor(QPalette::Link, QColor(42, 130, 218));
        palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
        palette.setColor(QPalette::HighlightedText, Qt::black);
    }
    else if (theme == "Light") {
        palette = QApplication::style()->standardPalette();  // Fix qApp reference
        palette.setColor(QPalette::Window, QColor(240, 240, 240));
        palette.setColor(QPalette::Base, QColor(255, 255, 255));
    }
    else {
        // Default theme - use system default
        palette = QApplication::style()->standardPalette();  // Fix qApp reference
    }
    
    QApplication::setPalette(palette);  // Fix qApp reference
}

void LauncherGui::showAboutDialog() {
    QMessageBox::about(this, "About XEmuRun",
                     "<h2>XEmuRun</h2>"
                     "<p>Universal Game Emulation Platform</p>"
                     "<p>Version 0.1.0</p>"
                     "<p>© 2023 XEmuRun Project</p>"
                     "<p>XEmuRun allows you to play games from various platforms "
                     "including Windows, Linux, PlayStation, and Xbox systems using "
                     "a unified interface.</p>");
}

void LauncherGui::openSettings() {
    m_tabWidget->setCurrentWidget(m_settingsTab);
}

void LauncherGui::scanForGames() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Directory to Scan for Game Packages");
    if (dir.isEmpty()) {
        return;
    }
    
    statusBar()->showMessage("Scanning for game packages...");
    
    int count = 0;
    QDirIterator it(dir, QStringList() << "*.XEmupkg", QDir::Files, QDirIterator::Subdirectories);
    
    while (it.hasNext()) {
        QString file = it.next();
        if (m_gameLibrary->addGame(file)) {
            count++;
        }
    }
    
    refreshLibrary();
    statusBar()->showMessage(QString("Found and imported %1 game packages").arg(count), 5000);
}

bool LauncherGui::importAndLaunchGame(const QString& packagePath) {
    // Add the game to the library if needed
    if (!m_gameLibrary->getGameInfo(packagePath).name.isEmpty()) {
        // Game already exists in library
        refreshLibrary();
        
        // Find and select the game in the list
        for (int i = 0; i < m_gamesList->count(); i++) {
            QListWidgetItem* item = m_gamesList->item(i);
            if (item->data(Qt::UserRole).toString() == packagePath) {
                m_gamesList->setCurrentItem(item);
                gameSelected(item);
                break;
            }
        }
    } else {
        // Import the new game
        if (!m_gameLibrary->addGame(packagePath)) {
            QMessageBox::warning(this, "Import Error", "Failed to import the game package.");
            return false;
        }
        
        refreshLibrary();
        
        // Find and select the newly added game
        for (int i = 0; i < m_gamesList->count(); i++) {
            QListWidgetItem* item = m_gamesList->item(i);
            if (item->data(Qt::UserRole).toString() == packagePath) {
                m_gamesList->setCurrentItem(item);
                gameSelected(item);
                break;
            }
        }
    }
    
    // Launch the game
    launchGame();
    return true;
}

} // namespace XEmuRun
