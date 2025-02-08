#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QSettings>
#include <QDirIterator>

class ConfigManager : public QObject
{
    Q_OBJECT
public:
    static ConfigManager& instance();

    // 公共访问方法
    QSettings* getSettings();
    QString configPath() const;
    QString GetConfigPath();
    QString GetHomePath();
    QString GetMusicFolderPath();
    QString GetHomeConfigPath();
    QString GetMusicFolderConfigPath();
    QString GetHomeLibraryPath();
    QString GetMusicFolderLibraryPath();

    // 常用配置项快捷访问
    QStringList musicDatabasePaths();
    int setConfig(QString key, QVariant value);

public slots:
    QString SaveConfig(QString configPath = QString(), QSettings* externalSettings = nullptr);

private:
    explicit ConfigManager(QObject *parent = nullptr);

    const QString configFileName = "intmusic.ini";
    const QString librarySubfix = "intlibrary";

    QSettings* m_settings = nullptr;

signals:
    void settingsChanged();

};

#endif // CONFIGMANAGER_H
