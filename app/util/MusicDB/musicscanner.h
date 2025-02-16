#ifndef SCANMUSIC_H
#define SCANMUSIC_H

#include <QObject>
#include <QDir>
#include <QDirIterator>
#include <QDebug>
#include <QFileInfo>
#include <QStringList>

#include <taglib/taglib.h>
#include <taglib/fileref.h>
#include <taglib/tag.h>

#include "app/util/Config/configmanager.h"
#include "app/util/Config/musiclibrary.h"


class MusicScanner : public QObject
{
    Q_OBJECT
public:
    static MusicScanner& instance();
    ~MusicScanner();

    QSettings* intSetting;

signals:
    void scanMusicStarted();
    void scanMusicFinished(int fileCount);
    void scanMusicFailed(const QString &error);
    void scanTagStarted();
    void scanTagFinished(int tagCount);
    void scanTagFailed(const QString &error);
    void musicFileTagRead(const QString &filePath, const QString &title, const QString &artist, const QString &album);


public slots:
    void startScan();

private:
    explicit MusicScanner(QObject *parent = nullptr);
    ConfigManager &configManager;
    QStringList m_supportedFormats = {"*.mp3", "*.flac", "*.wav", "*.ogg", "*.m4a"};
    int maxFiles = 10;
};

#endif // SCANMUSIC_H
