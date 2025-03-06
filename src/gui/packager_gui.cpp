#include "packager_gui.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QMessageBox>
#include <QHeaderView>
#include <QThread>
#include <QFileInfo>
#include <QDir>
#include <filesystem>
#include "../config/config_manager.h"

namespace fs = std::filesystem;

namespace XEmuRun {

PackagerGui::PackagerGui(QWidget *parent)
    : QMainWindow(parent), m_packager(std::make_unique<Packager>()) {
    setWindowTitle("XEmuRun Package Creator");
    setMinimumSize(600, 500);
    
    setupUi();
    connectSignalsSlots();
    
    // Initialize with system config
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        // Set default output directory if available
        std::string defaultOutputDir = configManager.getSystemConfig().getString("default_output_directory", "");
        if (!defaultOutputDir.empty()) {
            m_outputPathEdit->setText(QString::fromStdString(defaultOutputDir));
        }
    }
}

PackagerGui::~PackagerGui() = default;

void PackagerGui::setupUi() {
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);
    
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);
    
    // Game information group
    QGroupBox *gameInfoGroup = new QGroupBox("Game Information", centralWidget);
    QFormLayout *gameInfoLayout = new QFormLayout(gameInfoGroup);
    
    m_gamePathEdit = new QLineEdit(gameInfoGroup);
    m_gamePathButton = new QPushButton("Browse...", gameInfoGroup);
    QHBoxLayout *gamePathLayout = new QHBoxLayout();
    gamePathLayout->addWidget(m_gamePathEdit);
    gamePathLayout->addWidget(m_gamePathButton);
    
    m_outputPathEdit = new QLineEdit(gameInfoGroup);
    m_outputPathButton = new QPushButton("Browse...", gameInfoGroup);
    QHBoxLayout *outputPathLayout = new QHBoxLayout();
    outputPathLayout->addWidget(m_outputPathEdit);
    outputPathLayout->addWidget(m_outputPathButton);
    
    m_gameNameEdit = new QLineEdit(gameInfoGroup);
    
    m_platformComboBox = new QComboBox(gameInfoGroup);
    m_platformComboBox->addItem("windows");
    m_platformComboBox->addItem("linux");
    m_platformComboBox->addItem("playstation4");
    m_platformComboBox->addItem("playstation5");
    m_platformComboBox->addItem("xbox");
    m_platformComboBox->addItem("xbox_series");
    
    m_executableEdit = new QLineEdit(gameInfoGroup);
    m_executableButton = new QPushButton("Browse...", gameInfoGroup);
    QHBoxLayout *executableLayout = new QHBoxLayout();
    executableLayout->addWidget(m_executableEdit);
    executableLayout->addWidget(m_executableButton);
    
    gameInfoLayout->addRow("Game Path:", gamePathLayout);
    gameInfoLayout->addRow("Output Path:", outputPathLayout);
    gameInfoLayout->addRow("Game Name:", m_gameNameEdit);
    gameInfoLayout->addRow("Platform:", m_platformComboBox);
    gameInfoLayout->addRow("Main Executable:", executableLayout);
    
    mainLayout->addWidget(gameInfoGroup);
    
    // Configuration group
    QGroupBox *configGroup = new QGroupBox("Configuration", centralWidget);
    QVBoxLayout *configLayout = new QVBoxLayout(configGroup);
    
    m_configTableWidget = new QTableWidget(0, 2, configGroup);
    m_configTableWidget->setHorizontalHeaderLabels({"Key", "Value"});
    m_configTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    
    QHBoxLayout *configButtonLayout = new QHBoxLayout();
    m_addConfigButton = new QPushButton("Add", configGroup);
    m_removeConfigButton = new QPushButton("Remove", configGroup);
    configButtonLayout->addWidget(m_addConfigButton);
    configButtonLayout->addWidget(m_removeConfigButton);
    configButtonLayout->addStretch();
    
    configLayout->addWidget(m_configTableWidget);
    configLayout->addLayout(configButtonLayout);
    
    mainLayout->addWidget(configGroup);
    
    // Auto-detect button
    m_autoDetectButton = new QPushButton("Auto-detect Platform && Executable", centralWidget);
    mainLayout->addWidget(m_autoDetectButton);
    
    // Progress and status
    m_progressBar = new QProgressBar(centralWidget);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_progressBar->setTextVisible(true);
    mainLayout->addWidget(m_progressBar);
    
    m_statusLabel = new QLabel(centralWidget);
    m_statusLabel->setText("Ready");
    mainLayout->addWidget(m_statusLabel);
    
    // Create package button
    m_createPackageButton = new QPushButton("Create Package", centralWidget);
    m_createPackageButton->setEnabled(false);
    mainLayout->addWidget(m_createPackageButton);
}

void PackagerGui::connectSignalsSlots() {
    connect(m_gamePathEdit, &QLineEdit::textChanged, this, &PackagerGui::updateCreateButtonState);
    connect(m_outputPathEdit, &QLineEdit::textChanged, this, &PackagerGui::updateCreateButtonState);
    connect(m_gameNameEdit, &QLineEdit::textChanged, this, &PackagerGui::updateCreateButtonState);
    connect(m_executableEdit, &QLineEdit::textChanged, this, &PackagerGui::updateCreateButtonState);
    
    // Direct button connections
    connect(m_gamePathButton, &QPushButton::clicked, this, &PackagerGui::browseGamePath);
    connect(m_outputPathButton, &QPushButton::clicked, this, &PackagerGui::browseOutputPath);
    connect(m_executableButton, &QPushButton::clicked, this, &PackagerGui::browseExecutable);
    
    connect(m_addConfigButton, &QPushButton::clicked, this, &PackagerGui::addConfigRow);
    connect(m_removeConfigButton, &QPushButton::clicked, this, &PackagerGui::removeConfigRow);
    connect(m_createPackageButton, &QPushButton::clicked, this, &PackagerGui::createPackage);
    connect(m_autoDetectButton, &QPushButton::clicked, this, &PackagerGui::autoDetect);
    connect(m_platformComboBox, &QComboBox::currentTextChanged, this, &PackagerGui::platformChanged);
}

void PackagerGui::browseGamePath() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Game Directory");
    if (!dir.isEmpty()) {
        m_gamePathEdit->setText(dir);
        
        // Auto-extract game name from directory name
        QFileInfo fileInfo(dir);
        if (m_gameNameEdit->text().isEmpty()) {
            m_gameNameEdit->setText(fileInfo.fileName());
        }
    }
}

void PackagerGui::browseOutputPath() {
    QString dir = QFileDialog::getExistingDirectory(this, "Select Output Directory");
    if (!dir.isEmpty()) {
        m_outputPathEdit->setText(dir);
    }
}

void PackagerGui::browseExecutable() {
    if (m_gamePathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a game path first");
        return;
    }
    
    QString filter;
    if (m_platformComboBox->currentText() == "windows") {
        filter = "Executables (*.exe);;All Files (*)";
    } else {
        filter = "All Files (*)";
    }
    
    QString file = QFileDialog::getOpenFileName(this, "Select Main Executable", 
                                               m_gamePathEdit->text(), filter);
    
    if (!file.isEmpty()) {
        // Make the path relative to the game path
        QDir gameDir(m_gamePathEdit->text());
        m_executableEdit->setText(gameDir.relativeFilePath(file));
    }
}

void PackagerGui::addConfigRow() {
    int row = m_configTableWidget->rowCount();
    m_configTableWidget->insertRow(row);
    m_configTableWidget->setItem(row, 0, new QTableWidgetItem(""));
    m_configTableWidget->setItem(row, 1, new QTableWidgetItem(""));
}

void PackagerGui::removeConfigRow() {
    int row = m_configTableWidget->currentRow();
    if (row >= 0) {
        m_configTableWidget->removeRow(row);
    }
}

void PackagerGui::createPackage() {
    if (!validateInputs()) {
        return;
    }
    
    // Update packager with UI values
    m_packager->setGamePath(m_gamePathEdit->text().toStdString());
    m_packager->setOutputPath(m_outputPathEdit->text().toStdString());
    m_packager->setGameName(m_gameNameEdit->text().toStdString());
    m_packager->setPlatform(m_platformComboBox->currentText().toStdString());
    m_packager->setMainExecutable(m_executableEdit->text().toStdString());
    
    // Add config values
    for (int i = 0; i < m_configTableWidget->rowCount(); ++i) {
        QTableWidgetItem* keyItem = m_configTableWidget->item(i, 0);
        QTableWidgetItem* valueItem = m_configTableWidget->item(i, 1);
        
        if (keyItem && valueItem && !keyItem->text().isEmpty()) {
            m_packager->addConfigValue(keyItem->text().toStdString(), 
                                      valueItem->text().toStdString());
        }
    }
    
    // Disable UI during packaging
    m_createPackageButton->setEnabled(false);
    m_progressBar->setValue(0);
    setStatusMessage("Creating package...");
    
    // Use a separate thread for packaging to keep the UI responsive
    QThread* thread = QThread::create([this]() {
        bool success = m_packager->createPackage();
        return success;
    });
    
    connect(thread, &QThread::finished, this, [this, thread]() {
        // Re-enable UI
        m_createPackageButton->setEnabled(true);
        m_progressBar->setValue(100);
        
        bool success = thread->property("success").toBool();
        if (success) {
            setStatusMessage("Package created successfully!", false);
            QMessageBox::information(this, "Success", "Package created successfully!");
        } else {
            setStatusMessage("Failed to create package", true);
            QMessageBox::critical(this, "Error", "Failed to create package");
        }
        
        thread->deleteLater();
    });
    
    thread->start();
}

void PackagerGui::autoDetect() {
    QString gamePath = m_gamePathEdit->text();
    if (gamePath.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Please select a game path first");
        return;
    }
    
    setStatusMessage("Auto-detecting...");
    m_progressBar->setValue(25);
    
    // Check for Windows executables (.exe files)
    QDir gameDir(gamePath);
    QStringList exeFiles = gameDir.entryList(QStringList() << "*.exe", QDir::Files, QDir::Name);
    
    m_progressBar->setValue(50);
    
    if (!exeFiles.isEmpty()) {
        m_platformComboBox->setCurrentText("windows");
        m_executableEdit->setText(exeFiles.first());
        setStatusMessage("Detected Windows executable: " + exeFiles.first());
        m_progressBar->setValue(100);
        return;
    }
    
    m_progressBar->setValue(75);
    
    // Look for Linux executables (files with execute permission)
    try {
        for (const auto& entry : fs::recursive_directory_iterator(gamePath.toStdString())) {
            if (entry.is_regular_file()) {
                fs::perms p = fs::status(entry.path()).permissions();
                if ((p & fs::perms::owner_exec) != fs::perms::none) {
                    QString relPath = gameDir.relativeFilePath(QString::fromStdString(entry.path().string()));
                    m_platformComboBox->setCurrentText("linux");
                    m_executableEdit->setText(relPath);
                    setStatusMessage("Detected Linux executable: " + relPath);
                    m_progressBar->setValue(100);
                    return;
                }
            }
        }
    } catch (const fs::filesystem_error& e) {
        setStatusMessage("Error checking files: " + QString::fromStdString(e.what()), true);
        m_progressBar->setValue(100);
        return;
    }
    
    // No detection possible
    setStatusMessage("Could not auto-detect platform. Please specify manually.", true);
    m_progressBar->setValue(100);
}

void PackagerGui::updateCreateButtonState() {
    bool hasGamePath = !m_gamePathEdit->text().isEmpty();
    bool hasOutputPath = !m_outputPathEdit->text().isEmpty();
    bool hasGameName = !m_gameNameEdit->text().isEmpty();
    bool hasExecutable = !m_executableEdit->text().isEmpty();
    
    m_createPackageButton->setEnabled(hasGamePath && hasOutputPath && hasGameName && hasExecutable);
}

void PackagerGui::platformChanged(const QString& platform) {
    // Update any platform-specific UI elements
    if (platform == "windows") {
        // Set executable filter to .exe for Windows
        if (!m_executableEdit->text().endsWith(".exe") && !m_executableEdit->text().isEmpty()) {
            setStatusMessage("Warning: Non-EXE file selected for Windows platform", true);
        }
    }
}

bool PackagerGui::validateInputs() {
    // Check required fields
    if (m_gamePathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Game path is required");
        return false;
    }
    
    if (m_outputPathEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Output path is required");
        return false;
    }
    
    if (m_gameNameEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Game name is required");
        return false;
    }
    
    if (m_executableEdit->text().isEmpty()) {
        QMessageBox::warning(this, "Validation Error", "Main executable is required");
        return false;
    }
    
    // Check if paths exist
    QFileInfo gamePathInfo(m_gamePathEdit->text());
    if (!gamePathInfo.exists() || !gamePathInfo.isDir()) {
        QMessageBox::warning(this, "Validation Error", "Game path does not exist or is not a directory");
        return false;
    }
    
    QFileInfo outputPathInfo(m_outputPathEdit->text());
    if (!outputPathInfo.exists() || !outputPathInfo.isDir()) {
        QMessageBox::warning(this, "Validation Error", "Output path does not exist or is not a directory");
        return false;
    }
    
    // Check if executable exists within game path
    QFileInfo executableInfo(m_gamePathEdit->text() + "/" + m_executableEdit->text());
    if (!executableInfo.exists() || !executableInfo.isFile()) {
        QMessageBox::warning(this, "Validation Error", "Main executable does not exist within the game path");
        return false;
    }
    
    return true;
}

void PackagerGui::setStatusMessage(const QString& message, bool isError) {
    m_statusLabel->setText(message);
    
    if (isError) {
        m_statusLabel->setStyleSheet("color: red;");
    } else {
        m_statusLabel->setStyleSheet("");
    }
}

} // namespace XEmuRun
