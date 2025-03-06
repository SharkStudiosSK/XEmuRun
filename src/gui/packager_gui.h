#pragma once

#include <QMainWindow>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QTableWidget>
#include <QProgressBar>
#include <QLabel>
#include <QFileDialog>
#include <memory>
#include "../packager/packager.h"

namespace XEmuRun {

class PackagerGui : public QMainWindow {
    Q_OBJECT
    
public:
    PackagerGui(QWidget *parent = nullptr);
    ~PackagerGui();

private slots:
    void browseGamePath();
    void browseOutputPath();
    void browseExecutable();
    void addConfigRow();
    void removeConfigRow();
    void createPackage();
    void autoDetect();
    void updateCreateButtonState();
    void platformChanged(const QString& platform);
    
private:
    void setupUi();
    void connectSignalsSlots();
    bool validateInputs();
    void setStatusMessage(const QString& message, bool isError = false);
    
    // UI Elements
    QLineEdit* m_gamePathEdit;
    QLineEdit* m_outputPathEdit;
    QLineEdit* m_gameNameEdit;
    QComboBox* m_platformComboBox;
    QLineEdit* m_executableEdit;
    QTableWidget* m_configTableWidget;
    QPushButton* m_addConfigButton;
    QPushButton* m_removeConfigButton;
    QPushButton* m_createPackageButton;
    QPushButton* m_autoDetectButton;
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;
    
    // Browse buttons
    QPushButton* m_gamePathButton;
    QPushButton* m_outputPathButton;
    QPushButton* m_executableButton;
    
    // Packager
    std::unique_ptr<Packager> m_packager;
};

} // namespace XEmuRun
