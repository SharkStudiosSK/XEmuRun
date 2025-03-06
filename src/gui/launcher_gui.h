#pragma once

#include <QMainWindow>
#include <QTabWidget>
#include <QListWidget>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStackedWidget>
#include <QFileDialog>
#include <QMessageBox>
#include <QSettings>
#include <QComboBox>
#include <QTreeView>
#include <QStandardItemModel>
#include <memory>

#include "game_library.h"
#include "controller_mapping.h"
#include "../launcher/launcher.h"

namespace XEmuRun {

class LauncherGui : public QMainWindow {
    Q_OBJECT
    
public:
    LauncherGui(QWidget *parent = nullptr);
    ~LauncherGui();
    
    // Import and launch a game package directly
    bool importAndLaunchGame(const QString& packagePath);

private slots:
    void importGame();
    void launchGame();
    void refreshLibrary();
    void gameSelected(QListWidgetItem* item);
    void showGameDetails(const GameInfo& game);
    void applySettings();
    void showAboutDialog();
    void openSettings();
    void scanForGames();
    void applyTheme(const QString& theme);

private:
    void setupUi();
    void loadSettings();
    void saveSettings();
    void connectSignals();
    void populatePlatforms();
    void setupMenus();
    
    // UI components
    QTabWidget* m_tabWidget;
    
    // Library tab
    QWidget* m_libraryTab;
    GameLibrary* m_gameLibrary;
    QListWidget* m_gamesList;
    QPushButton* m_importButton;
    QPushButton* m_launchButton;
    QPushButton* m_refreshButton;
    QLabel* m_gameInfoLabel;
    QLabel* m_gameIconLabel;
    QLabel* m_gamePlatformLabel;
    
    // Controllers tab
    QWidget* m_controllersTab;
    ControllerMapping* m_controllerMapping;
    
    // Settings tab
    QWidget* m_settingsTab;
    QGroupBox* m_generalSettingsGroup;
    QGroupBox* m_emulatorPathsGroup;
    QComboBox* m_themeCombo;
    QTreeView* m_emulatorPathsView;
    QStandardItemModel* m_emulatorPathsModel;
    QPushButton* m_applySettingsButton;
    
    // Status bar
    QLabel* m_statusLabel;
    
    // Game launcher
    std::unique_ptr<Launcher> m_launcher;
};

} // namespace XEmuRun
