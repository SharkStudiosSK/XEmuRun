#include "game_library.h"
#include <QStandardPaths>
#include <QTemporaryDir>
#include <QProcess>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <json/json.h>
#include "../utils/archive.h"

namespace fs = std::filesystem;

namespace XEmuRun {

GameLibrary::GameLibrary(QObject* parent) : QObject(parent) {
    // Set up library path in user's data location
    QString dataPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
    QDir dir(dataPath);
    
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    m_libraryPath = dir.filePath("library.json");
    
    // Load existing library
    loadLibrary();
}

GameLibrary::~GameLibrary() {
    saveLibrary();
}

bool GameLibrary::addGame(const QString& packagePath) {
    if (!QFile::exists(packagePath)) {
        return false;
    }
    
    // Extract game info
    GameInfo info;
    if (!extractGameInfo(packagePath, info)) {
        return false;
    }
    
    // Add to library
    m_games[packagePath] = info;
    
    // Save changes
    saveLibrary();
    
    return true;
}

bool GameLibrary::removeGame(const QString& packagePath) {
    if (!m_games.contains(packagePath)) {
        return false;
    }
    
    m_games.remove(packagePath);
    return saveLibrary();
}

QList<GameInfo> GameLibrary::getGames() const {
    return m_games.values();
}

GameInfo GameLibrary::getGameInfo(const QString& packagePath) const {
    if (m_games.contains(packagePath)) {
        return m_games[packagePath];
    }
    
    return GameInfo();
}

bool GameLibrary::loadLibrary() {
    QFile file(m_libraryPath);
    if (!file.exists() || !file.open(QIODevice::ReadOnly)) {
        return false;
    }
    
    QByteArray data = file.readAll();
    file.close();
    
    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        return false;
    }
    
    QJsonObject obj = doc.object();
    
    // Clear existing games
    m_games.clear();
    
    // Load games
    for (auto it = obj.begin(); it != obj.end(); ++it) {
        QString packagePath = it.key();
        QJsonObject gameObj = it.value().toObject();
        
        GameInfo info;
        info.packagePath = packagePath;
        info.name = gameObj["name"].toString();
        info.platform = gameObj["platform"].toString();
        info.iconPath = gameObj["iconPath"].toString();
        info.description = gameObj["description"].toString();
        info.version = gameObj["version"].toString();
        info.mainExecutable = gameObj["mainExecutable"].toString();
        
        m_games[packagePath] = info;
    }
    
    return true;
}

bool GameLibrary::saveLibrary() {
    QFile file(m_libraryPath);
    if (!file.open(QIODevice::WriteOnly)) {
        return false;
    }
    
    QJsonObject obj;
    
    // Save games
    for (auto it = m_games.begin(); it != m_games.end(); ++it) {
        QJsonObject gameObj;
        gameObj["name"] = it.value().name;
        gameObj["platform"] = it.value().platform;
        gameObj["iconPath"] = it.value().iconPath;
        gameObj["description"] = it.value().description;
        gameObj["version"] = it.value().version;
        gameObj["mainExecutable"] = it.value().mainExecutable;
        
        obj[it.key()] = gameObj;
    }
    
    QJsonDocument doc(obj);
    file.write(doc.toJson());
    file.close();
    
    return true;
}

bool GameLibrary::extractGameInfo(const QString& packagePath, GameInfo& info) {
    QTemporaryDir tempDir;
    if (!tempDir.isValid()) {
        return false;
    }
    
    // Extract manifest.json from the package
    try {
        if (!extractArchive(packagePath.toStdString(), tempDir.path().toStdString())) {
            return false;
        }
        
        // Read manifest.json
        QFile manifestFile(tempDir.filePath("manifest.json"));
        if (!manifestFile.open(QIODevice::ReadOnly)) {
            return false;
        }
        
        QByteArray manifestData = manifestFile.readAll();
        manifestFile.close();
        
        Json::Value root;
        Json::Reader reader;
        
        if (!reader.parse(manifestData.constData(), root)) {
            return false;
        }
        
        // Fill GameInfo
        info.packagePath = packagePath;
        info.name = QString::fromStdString(root["name"].asString());
        info.platform = QString::fromStdString(root["platform"].asString());
        info.mainExecutable = QString::fromStdString(root["main"].asString());
        
        // Optional fields
        if (root.isMember("version")) {
            info.version = QString::fromStdString(root["version"].asString());
        } else {
            info.version = "1.0.0";
        }
        
        if (root.isMember("description")) {
            info.description = QString::fromStdString(root["description"].asString());
        }
        
        // Look for an icon in the package
        QString iconPath = tempDir.filePath("icon.png");
        if (QFile::exists(iconPath)) {
            // Copy to a permanent location
            QString targetIconPath = QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) +
                                    "/icons/" + QFileInfo(packagePath).baseName() + ".png";
            
            QDir().mkpath(QFileInfo(targetIconPath).path());
            
            if (QFile::copy(iconPath, targetIconPath)) {
                info.iconPath = targetIconPath;
            }
        }
        
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Error extracting game info: " << e.what() << std::endl;
        return false;
    }
}

} // namespace XEmuRun
