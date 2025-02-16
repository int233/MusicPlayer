#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>

#include "app/util/Config/configmanager.h"

class MusicLibrary : public QObject
{
    Q_OBJECT

public:
    static MusicLibrary& instance();
    ~MusicLibrary();

    struct SongInfo {
        QString title;
        int durationMs;
        int fileSize;
        QString filePath;
        int albumId = -1;
        QString albumName;
        QString publisher;
        QString albumVersion;
        QDateTime albumReleaseTime;
        int totalTracks = 0;
        int totalDiscs = 0;
        int trackNumber = 1;
        int discNumber = 1;
        bool isFavorite = false;
        bool isLive = false;
        bool isSingle = false;
        QStringList language;
        QString liveEvent;
        QString liveEventCount;
        bool isCover = false;
        bool isMashup = false;
        QDateTime releaseTime;
        QMultiMap<QString, QPair<QString, QString>> characters; // key: role, value: (characterName, foreignName)
        QStringList genres;
        QStringList languages;
    };

    struct SongItem {
        QString title;
        QStringList artists;
        int durationMs;
        QString filePath;
    };

    // 返回歌曲
    std::vector<SongItem> getAllSongs();

    // 插入歌曲
    bool insertSong(const SongInfo &songInfo);

private:
    explicit MusicLibrary(QObject *parent = nullptr);
    ConfigManager &configManager;

    QStringList getLibraryPaths();

    QSqlDatabase m_db;
    void initializeDatabase();
    void createTables();

    // 数据库连接配置
    static constexpr const char* DB_CONNECTION_NAME = "MUSIC_LIB_CONN";
    // static constexpr const char* DB_FILE_NAME = "/music_library.db";

    // 插入角色
    int ensureCharacterExists(const QString &characterName, const QString &foreignName);
    // 检查专辑是否存在
    int ensureAlbumExists(const SongInfo &songInfo);

signals:

};

#endif // MUSICLIBRARY_H
