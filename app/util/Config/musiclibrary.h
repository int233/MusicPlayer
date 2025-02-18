#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QRegularExpression>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

#include "app/util/Config/configmanager.h"

class MusicLibrary : public QObject
{
    Q_OBJECT

public:
    static MusicLibrary& instance();
    ~MusicLibrary();

    QString splitPartten = "[;,/]";

    struct SongInfo {
        QString title;
        int durationMs;
        int fileSize;
        QString filePath;
        int albumId = -1;
        int totalTracks = 0;
        int totalDiscs = 0;
        int trackNumber = 1;
        int discNumber = 1;
        bool isFavorite = false;
        bool isLive = false;
        bool isSingle = false;
        QString liveEvent;
        QString liveEventCount;
        bool isCover = false;
        bool isMashup = false;
        QDateTime releaseTime;
        QMultiMap<QString, QPair<QString, QString>> characters; // key: role, value: (characterName, foreignName)
        QStringList genres;
        QStringList languages;
    };

    struct DateInfo {
        QDate date = QDate(1, 1, 1);
        // 1: year, 2: year/month, 3: year/month/day
        int level = 0;
        QString toString() const {
            switch(level) {
            case 0: return "None";
            case 1: return date.toString("yyyy");
            case 2: return date.toString("yyyy-MM");
            case 3: return date.toString("yyyy-MM-dd");
            default: return date.toString();
            }
        };
    };

    struct AlbumInfo {
        int albumId = -1;
        // ARTIST
        // TagLib::Tag::album()
        QString albumName;
        // ALBUM ARTIST
        // file.file()->properties()["ALBUM ARTIST"]
        QStringList albumArtists;
        // PUBLISHER
        // file.file()->properties()["PUBLISHER"]
        QStringList publishers;
        // ALBUMVERSION
        // file.file()->properties()["ALBUMVERSION"]
        QString albumVersion;
        QString albumCoverPath;
        DateInfo originalReleaseTime;
        // RELEASE TIME
        // TagLib::Tag::year()
        DateInfo albumReleaseTime;
        // COUNTRY
        // file.file()->properties()["COUNTRY"]
        QString country;
        // TOTALDISCS
        // file.file()->properties()["TOTALDISCS"]
        int totalDiscs;
        QVector<int> tracksCounts;
        QStringList tracksTitles;
        QVector<DateInfo> tracksReleaseTimes;
        QVector<DateInfo> tracksStartTimes;
        QVector<DateInfo> tracksEndTimes;
        QStringList tracksCoverPaths;
        // LANGUAGE
        // file.file()->properties()["LANGUAGE"]
        QStringList languages;
        // GENRE
        // TagLib::Tag::genre()
        QStringList genres;
        // LIVEEVENT
        // file.file()->properties()["LIVEEVENT"]
        QString liveEvent;
        // LIVEEVENTCOUNT
        // file.file()->properties()["LIVEEVENTCOUNT"]
        QString liveEventCount;
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

    // 插入扫描结果
    bool insertScanResult(const QStringList &musicFiles);

private:
    explicit MusicLibrary(QObject *parent = nullptr);
    ConfigManager &configManager;

    QStringList getLibraryPaths() const;

    QSqlDatabase m_db;
    void initializeDatabase();
    void createTables();

    // 数据库连接配置
    static constexpr const char* DB_CONNECTION_NAME = "MUSIC_LIB_CONN";

    // 插入角色
    int ensureCharacterExists(const QString &characterName, const QString &foreignName);
    // 检查专辑是否存在
    int ensureAlbumExists(const SongInfo &songInfo);
    // 读取音乐标签
    MusicLibrary::AlbumInfo readAlbumInfo(const QString &filePath);
    // MusicLibrary::CharacterInfo readCharacterInfo(const QString &filePath);
    MusicLibrary::SongInfo readSongInfo(const QString &filePath);
    // 扫描专辑信息
    QHash<QString, AlbumInfo> getAlbumsInfo(const QStringList &musicFiles);
    // 扫描人物信息
    // QHash<QString, CharacterInfo> getCharactersInfo(const QStringList &musicFiles);
    // 扫描歌曲信息
    // QHash<QString, SongInfo> getSongsInfo(const QStringList &musicFiles);


    void closeDatabase();

    // 分割字符串
    QStringList splitString(const QString &str);
    QStringList splitString(const QStringList &strlist);
    // 格式化年月日
    DateInfo formatDate(const QString &dateString);

signals:

};

#endif // MUSICLIBRARY_H
