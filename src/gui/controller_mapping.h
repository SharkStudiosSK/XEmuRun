#pragma once

#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QTableWidget>
#include <QTimer>
#include <QMap>
#include <memory>
#include <SDL2/SDL.h>

namespace XEmuRun {

class ControllerMapping : public QWidget {
    Q_OBJECT
    
public:
    explicit ControllerMapping(QWidget* parent = nullptr);
    ~ControllerMapping();
    
public slots:
    void refreshDevices();
    void saveMapping();
    void loadMapping();
    
private slots:
    void startMapping();
    void cancelMapping();
    void controllerSelected(int index);
    void platformSelected(int index);
    void mappingItemClicked(QTableWidgetItem* item);
    void pollControllerInput();
    
private:
    void setupUi();
    void initializeSDL();
    void shutdownSDL();
    void updateMappingTable();
    void highlightCurrentMapping(int row);
    void applyCapturedMapping(int row, SDL_GameControllerButton button);
    QString getButtonName(int buttonId);
    
    // UI elements
    QComboBox* m_controllerCombo;
    QComboBox* m_platformCombo;
    QPushButton* m_refreshButton;
    QTableWidget* m_mappingTable;
    QLabel* m_statusLabel;
    QPushButton* m_startMappingButton;
    QPushButton* m_cancelMappingButton;
    QPushButton* m_saveButton;
    QPushButton* m_loadButton;
    
    // Controller data
    struct ControllerInfo {
        int id;
        QString name;
        SDL_GameController* controller = nullptr;
    };
    
    QVector<ControllerInfo> m_controllers;
    QMap<QString, QString> m_currentMapping;  // Maps action -> button
    
    // Mapping state
    bool m_isMapping;
    int m_mappingRow;
    QTimer* m_pollTimer;
};

} // namespace XEmuRun
