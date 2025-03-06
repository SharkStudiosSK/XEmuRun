#pragma once

#include <QString>
#include <QList>
#include <QHash>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QJsonDocument>
#include <QJsonObject>

namespace XEmuRun {

struct GameInfo {
    QString name;
    QString packagePath;
    QString platform;
    QString iconPath;
    QString description;
    QString version;
    QString mainExecutable;
};

class GameLibrary : public QObject {
    Q_OBJECT
    
public:
    explicit GameLibrary(QObject* parent = nullptr);
    ~GameLibrary();
    
    bool addGame(const QString& packagePath);
    bool removeGame(const QString& packagePath);
    QList<GameInfo> getGames() const;
    GameInfo getGameInfo(const QString& packagePath) const;
    
private:
    bool loadLibrary();
    bool saveLibrary();
    bool extractGameInfo(const QString& packagePath, GameInfo& info);
    
    QString m_libraryPath;
    QHash<QString, GameInfo> m_games;
};

} // namespace XEmuRun
