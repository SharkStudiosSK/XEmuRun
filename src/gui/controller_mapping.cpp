#include "controller_mapping.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QHeaderView>
#include <QMessageBox>
#include <QFileDialog>
#include <QSettings>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include "../config/config_manager.h"

namespace XEmuRun {

ControllerMapping::ControllerMapping(QWidget* parent) 
    : QWidget(parent), m_isMapping(false), m_mappingRow(-1)
{
    setupUi();
    initializeSDL();
    refreshDevices();
    
    // Create timer for controller polling
    m_pollTimer = new QTimer(this);
    connect(m_pollTimer, &QTimer::timeout, this, &ControllerMapping::pollControllerInput);
}

ControllerMapping::~ControllerMapping() {
    shutdownSDL();
}

void ControllerMapping::setupUi() {
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Controller selection
    QGroupBox* deviceGroup = new QGroupBox("Controller Device");
    QVBoxLayout* deviceLayout = new QVBoxLayout(deviceGroup);
    
    QHBoxLayout* controllerLayout = new QHBoxLayout();
    m_controllerCombo = new QComboBox();
    m_refreshButton = new QPushButton("Refresh");
    controllerLayout->addWidget(m_controllerCombo, 1);
    controllerLayout->addWidget(m_refreshButton);
    
    deviceLayout->addLayout(controllerLayout);
    
    // Platform selection
    QFormLayout* platformLayout = new QFormLayout();
    m_platformCombo = new QComboBox();
    m_platformCombo->addItem("windows");
    m_platformCombo->addItem("linux");
    m_platformCombo->addItem("playstation4");
    m_platformCombo->addItem("playstation5");
    m_platformCombo->addItem("xbox");
    m_platformCombo->addItem("xbox_series");
    platformLayout->addRow("Platform:", m_platformCombo);
    
    deviceLayout->addLayout(platformLayout);
    
    mainLayout->addWidget(deviceGroup);
    
    // Mapping table
    QGroupBox* mappingGroup = new QGroupBox("Button Mappings");
    QVBoxLayout* mappingLayout = new QVBoxLayout(mappingGroup);
    
    m_mappingTable = new QTableWidget(0, 2);
    m_mappingTable->setHorizontalHeaderLabels({"Action", "Button"});
    m_mappingTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    m_mappingTable->setSelectionBehavior(QTableWidget::SelectRows);
    
    // Add standard mappings
    QStringList actions = {
        "A", "B", "X", "Y", 
        "Back", "Guide", "Start",
        "Left Stick", "Right Stick",
        "Left Shoulder", "Right Shoulder",
        "DPad Up", "DPad Down", "DPad Left", "DPad Right",
        "Left Trigger", "Right Trigger"
    };
    
    m_mappingTable->setRowCount(actions.size());
    for (int i = 0; i < actions.size(); ++i) {
        m_mappingTable->setItem(i, 0, new QTableWidgetItem(actions[i]));
        m_mappingTable->setItem(i, 1, new QTableWidgetItem(""));
    }
    
    mappingLayout->addWidget(m_mappingTable);
    
    // Status and buttons
    m_statusLabel = new QLabel("Ready");
    mappingLayout->addWidget(m_statusLabel);
    
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    m_startMappingButton = new QPushButton("Start Mapping");
    m_cancelMappingButton = new QPushButton("Cancel");
    m_saveButton = new QPushButton("Save Mapping");
    m_loadButton = new QPushButton("Load Mapping");
    m_cancelMappingButton->setEnabled(false);
    
    buttonLayout->addWidget(m_startMappingButton);
    buttonLayout->addWidget(m_cancelMappingButton);
    buttonLayout->addWidget(m_saveButton);
    buttonLayout->addWidget(m_loadButton);
    
    mappingLayout->addLayout(buttonLayout);
    
    mainLayout->addWidget(mappingGroup);
    
    // Connect signals
    connect(m_refreshButton, &QPushButton::clicked, this, &ControllerMapping::refreshDevices);
    connect(m_startMappingButton, &QPushButton::clicked, this, &ControllerMapping::startMapping);
    connect(m_cancelMappingButton, &QPushButton::clicked, this, &ControllerMapping::cancelMapping);
    connect(m_saveButton, &QPushButton::clicked, this, &ControllerMapping::saveMapping);
    connect(m_loadButton, &QPushButton::clicked, this, &ControllerMapping::loadMapping);
    connect(m_controllerCombo, QOverload<int>::of(&QComboBox::currentIndexChanged), 
            this, &ControllerMapping::controllerSelected);
    connect(m_platformCombo, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &ControllerMapping::platformSelected);
    connect(m_mappingTable, &QTableWidget::itemClicked,
            this, &ControllerMapping::mappingItemClicked);
}

void ControllerMapping::initializeSDL() {
    if (SDL_Init(SDL_INIT_GAMECONTROLLER) < 0) {
        std::cerr << "Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return;
    }
    
    SDL_GameControllerEventState(SDL_ENABLE);
}

void ControllerMapping::shutdownSDL() {
    // Close any open controllers
    for (auto& controller : m_controllers) {
        if (controller.controller) {
            SDL_GameControllerClose(controller.controller);
        }
    }
    
    SDL_Quit();
}

void ControllerMapping::refreshDevices() {
    // Close existing controllers
    for (auto& controller : m_controllers) {
        if (controller.controller) {
            SDL_GameControllerClose(controller.controller);
            controller.controller = nullptr;
        }
    }
    
    m_controllers.clear();
    m_controllerCombo->clear();
    
    // Get number of joysticks/controllers
    int numJoysticks = SDL_NumJoysticks();
    
    for (int i = 0; i < numJoysticks; ++i) {
        if (SDL_IsGameController(i)) {
            ControllerInfo info;
            info.id = i;
            info.controller = SDL_GameControllerOpen(i);
            
            if (info.controller) {
                info.name = QString::fromUtf8(SDL_GameControllerName(info.controller));
                m_controllers.append(info);
                
                // Add to combo box
                m_controllerCombo->addItem(info.name);
            }
        }
    }
    
    m_startMappingButton->setEnabled(!m_controllers.isEmpty());
}

void ControllerMapping::startMapping() {
    if (m_controllers.isEmpty() || m_controllerCombo->currentIndex() < 0) {
        m_statusLabel->setText("No controller selected");
        return;
    }
    
    m_isMapping = true;
    m_statusLabel->setText("Press a button to map...");
    m_startMappingButton->setEnabled(false);
    m_cancelMappingButton->setEnabled(true);
    
    // Start with the first mapping
    m_mappingRow = 0;
    highlightCurrentMapping(m_mappingRow);
    
    // Start polling for controller input
    m_pollTimer->start(100);  // Poll every 100ms
}

void ControllerMapping::cancelMapping() {
    m_isMapping = false;
    m_mappingRow = -1;
    m_pollTimer->stop();
    
    m_statusLabel->setText("Mapping cancelled");
    m_startMappingButton->setEnabled(true);
    m_cancelMappingButton->setEnabled(false);
    
    // Remove highlight
    for (int i = 0; i < m_mappingTable->rowCount(); ++i) {
        m_mappingTable->item(i, 0)->setBackground(Qt::white);
        m_mappingTable->item(i, 1)->setBackground(Qt::white);
    }
}

void ControllerMapping::controllerSelected(int index) {
    if (index >= 0 && index < m_controllers.size()) {
        m_statusLabel->setText(QString("Selected controller: %1").arg(m_controllers[index].name));
    }
}

void ControllerMapping::platformSelected(int index) {
    QString platform = m_platformCombo->itemText(index);
    // Load platform-specific mappings
    loadMapping();
}

void ControllerMapping::mappingItemClicked(QTableWidgetItem* item) {
    if (m_isMapping) {
        return;  // Ignore clicks during active mapping
    }
    
    int row = item->row();
    m_mappingRow = row;
    
    m_statusLabel->setText(QString("Press a button to map to '%1'").arg(m_mappingTable->item(row, 0)->text()));
    highlightCurrentMapping(row);
    
    m_isMapping = true;
    m_startMappingButton->setEnabled(false);
    m_cancelMappingButton->setEnabled(true);
    
    // Start polling for controller input
    m_pollTimer->start(100);
}

void ControllerMapping::pollControllerInput() {
    SDL_Event event;
    
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_CONTROLLERBUTTONDOWN) {
            SDL_GameControllerButton button = (SDL_GameControllerButton)event.cbutton.button;
            
            if (m_isMapping && m_mappingRow >= 0) {
                applyCapturedMapping(m_mappingRow, button);
                
                // Move to next mapping
                m_mappingRow++;
                if (m_mappingRow >= m_mappingTable->rowCount()) {
                    // Finished mapping all buttons
                    m_isMapping = false;
                    m_mappingRow = -1;
                    m_pollTimer->stop();
                    
                    m_statusLabel->setText("Mapping complete");
                    m_startMappingButton->setEnabled(true);
                    m_cancelMappingButton->setEnabled(false);
                    
                    // Remove highlight
                    for (int i = 0; i < m_mappingTable->rowCount(); ++i) {
                        m_mappingTable->item(i, 0)->setBackground(Qt::white);
                        m_mappingTable->item(i, 1)->setBackground(Qt::white);
                    }
                } else {
                    // Highlight next row
                    highlightCurrentMapping(m_mappingRow);
                }
            }
        }
    }
}

void ControllerMapping::highlightCurrentMapping(int row) {
    // Reset all highlighting
    for (int i = 0; i < m_mappingTable->rowCount(); ++i) {
        m_mappingTable->item(i, 0)->setBackground(Qt::white);
        m_mappingTable->item(i, 1)->setBackground(Qt::white);
    }
    
    // Highlight selected row
    if (row >= 0 && row < m_mappingTable->rowCount()) {
        m_mappingTable->item(row, 0)->setBackground(Qt::yellow);
        m_mappingTable->item(row, 1)->setBackground(Qt::yellow);
    }
}

void ControllerMapping::applyCapturedMapping(int row, SDL_GameControllerButton button) {
    QString buttonName = getButtonName(button);
    
    if (row >= 0 && row < m_mappingTable->rowCount()) {
        QString action = m_mappingTable->item(row, 0)->text();
        m_mappingTable->item(row, 1)->setText(buttonName);
        
        // Store the mapping
        m_currentMapping[action] = buttonName;
    }
}

QString ControllerMapping::getButtonName(int buttonId) {
    switch (buttonId) {
        case SDL_CONTROLLER_BUTTON_A: return "A";
        case SDL_CONTROLLER_BUTTON_B: return "B";
        case SDL_CONTROLLER_BUTTON_X: return "X";
        case SDL_CONTROLLER_BUTTON_Y: return "Y";
        case SDL_CONTROLLER_BUTTON_BACK: return "Back";
        case SDL_CONTROLLER_BUTTON_GUIDE: return "Guide";
        case SDL_CONTROLLER_BUTTON_START: return "Start";
        case SDL_CONTROLLER_BUTTON_LEFTSTICK: return "Left Stick";
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK: return "Right Stick";
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER: return "Left Shoulder";
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER: return "Right Shoulder";
        case SDL_CONTROLLER_BUTTON_DPAD_UP: return "DPad Up";
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN: return "DPad Down";
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT: return "DPad Left";
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT: return "DPad Right";
        default: return QString("Button %1").arg(buttonId);
    }
}

void ControllerMapping::saveMapping() {
    if (m_currentMapping.isEmpty()) {
        QMessageBox::warning(this, "Save Mapping", "No mappings to save");
        return;
    }
    
    QString platform = m_platformCombo->currentText();
    QString controllerName = m_controllerCombo->currentText();
    
    if (controllerName.isEmpty()) {
        QMessageBox::warning(this, "Save Mapping", "No controller selected");
        return;
    }
    
    // Save to config system
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        Config& emulatorConfig = configManager.getEmulatorConfig(platform.toStdString());
        
        // Create JSON mapping
        QJsonObject mapping;
        for (auto it = m_currentMapping.constBegin(); it != m_currentMapping.constEnd(); ++it) {
            mapping[it.key()] = it.value();
        }
        
        QJsonDocument doc(mapping);
        QString mappingString = QString::fromUtf8(doc.toJson(QJsonDocument::Compact));
        
        // Store in config
        emulatorConfig.setString("controller_mapping_" + controllerName.toStdString(), 
                                mappingString.toStdString());
        
        configManager.saveEmulatorConfig(platform.toStdString());
        
        m_statusLabel->setText("Mapping saved successfully");
    }
}

void ControllerMapping::loadMapping() {
    QString platform = m_platformCombo->currentText();
    QString controllerName = m_controllerCombo->currentText();
    
    if (controllerName.isEmpty()) {
        m_statusLabel->setText("No controller selected");
        return;
    }
    
    // Load from config system
    ConfigManager& configManager = ConfigManager::getInstance();
    if (configManager.initialize()) {
        Config& emulatorConfig = configManager.getEmulatorConfig(platform.toStdString());
        
        std::string mappingString = emulatorConfig.getString(
            "controller_mapping_" + controllerName.toStdString(), "{}");
        
        // Parse JSON
        QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(mappingString));
        if (doc.isNull() || !doc.isObject()) {
            m_statusLabel->setText("No saved mapping found");
            return;
        }
        
        QJsonObject mapping = doc.object();
        
        // Clear current mapping
        m_currentMapping.clear();
        
        // Reset table
        for (int i = 0; i < m_mappingTable->rowCount(); ++i) {
            m_mappingTable->item(i, 1)->setText("");
        }
        
        // Apply loaded mappings
        for (auto it = mapping.constBegin(); it != mapping.constEnd(); ++it) {
            QString action = it.key();
            QString button = it.value().toString();
            
            m_currentMapping[action] = button;
            
            // Update table
            for (int i = 0; i < m_mappingTable->rowCount(); ++i) {
                if (m_mappingTable->item(i, 0)->text() == action) {
                    m_mappingTable->item(i, 1)->setText(button);
                    break;
                }
            }
        }
        
        m_statusLabel->setText("Mapping loaded successfully");
    }
}

} // namespace XEmuRun
