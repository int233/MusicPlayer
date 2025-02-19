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
#include <QThread>

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
        int albumID = -1;
        // ARTIST
        // TagLib::Tag::album()
        QString albumName;
        // ALBUM ARTIST
        // file.file()->properties()["ALBUM ARTIST"]
        QStringList albumArtists;
        // PUBLISHER
        // file.file()->properties()["PUBLISHER"]
        QStringList publishers;
        // ALBUM SCORE
        // file.file()->properties()["ALBUM SCORE"]
        int albumScore = 0;
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
    struct musiclibrary
    {
        // ARTIST
        // 艺术家
        QStringList artists;
        // ALBUM ARTIST
        // 专辑艺术家
        QStringList albumArtists;
        // COMPOSER
        // 作曲
        QStringList composers;
        // LYRICIST
        // 作词
        QStringList lyricists;
        // PERFORMER
        // 指挥
        QStringList performers;
    };
    struct CharacterInfo{
        int characterID = -1;
        QString characterName;
        QString foreignName;
        QString role;
        CharacterInfo(const QString& role,const QString& name, const QString& foreign)
        : role(role){
            // 检查是否仅有一个值有内容
            bool hasName = !name.isEmpty();
            bool hasForeign = !foreign.isEmpty();
    
            if (hasName && !hasForeign) {
                // 如果只有 characterName 有值，且不包含中文字符，则放入 foreignName
                if (!containsChinese(name)) {
                    foreignName = name;
                    characterName.clear();
                } else {
                    characterName = name;
                }
            } else if (!hasName && hasForeign) {
                // 如果只有 foreignName 有值，且不包含中文字符，则放入 foreignName
                if (!containsChinese(foreign)) {
                    foreignName = foreign;
                } else {
                    characterName = foreign;
                    foreignName.clear();
                }
            } else {
                // 如果两个值都有值或都没有值，则直接赋值
                characterName = name;
                foreignName = foreign;
            }
        }
        private:
            // 检查字符串是否包含中文字符
            static bool containsChinese(const QString& text) {
                QRegularExpression regex("[\u4e00-\u9fff]"); // 中文字符的 Unicode 范围
                return regex.match(text).hasMatch();
            }
    };

    struct SongInfo {
        QString title;
        QStringList artists;
        // 时长(s)
        int durations;
        // 文件大小(KB)
        int fileSize;
        QString filePath;
        // 专辑ID: 单曲设置为0，存在专辑则设置为专辑ID
        int albumID = -1;
        int totalTracks = 0;
        int totalDiscs = 0;
        int trackNumber = 0;
        int discNumber = 0;
        // 是否喜爱
        // FAVORITE、LOVE RATING、LOVE任意一个标签中有值则为喜爱
        bool isFavorite = false;
        // 是否现场
        // LIVE标签有值则为现场
        bool isLive = false;
        // 是否单曲
        // SINGLE标签有值则为单曲
        bool isSingle = false;
        QString liveEvent;
        QString liveEventCount;
        // 是否翻唱
        // COVER标签有值则为翻唱
        bool isCover = false;
        // 是否串烧
        // MASHUP标签有值则为串烧
        bool isMashup = false;
        // ORDATE
        DateInfo originalReleaseDate;
        // RDATE
        DateInfo releaseDate;
        QVector<CharacterInfo> characters; // key: role, value: (characterName, foreignName)
        // tag->genre()
        QStringList genres;
        // LANGUAGE
        // file.file()->properties()["LANGUAGE"]
        QStringList languages;
    };

    struct SongItem {
        QString title;
        QStringList artists;
        int duration;
        QString album;
        QString filePath;
    };

    // 返回歌曲
    std::vector<SongItem> getAllSongs();

    // 插入歌曲
    // bool insertSong(const SongInfo &songInfo);

    // 插入扫描结果
    bool insertScanResult(const QStringList &musicFiles);
    // 从数据库中解析插入结果
    QStringList retrieveMusicFiles() const;
    bool parseInsertResult();

private:
    explicit MusicLibrary(QObject *parent = nullptr);
    ConfigManager &configManager;

    QStringList getLibraryPaths() const;

    // state
    // 0: not start;
    // 1: is_scanning;
    // 2: getAlbumsInfo;
    // 3: getCharactersInfo;
    // 4: ensureAlbumExists;
    // 5: ensureCharacterExists;
    // 6: getSongsInfo;
    // 7: ensureSongExists;
    // 8: finnish
    int state = 0;

    QSqlDatabase m_db;
    void initializeDatabase();
    void createTables();

    // 数据库连接配置
    static constexpr const char* DB_CONNECTION_NAME = "MUSIC_LIB_CONN";

    // 插入角色
    int ensureCharacterExists(const QString &characterName, const QString &foreignName);
    // 插入角色和歌曲的关系
    int pairSongCharacter(const int &songID, const int &characterID, const QString &role);
    // 插入流派和歌曲的关系
    int pairSongGenre(const int &songID, const QString &genre);
    // 插入语言和歌曲的关系
    int pairSongLanguage(const int &songID, const QString &genre);
    // 检查专辑是否存在，并插入不存在的专辑
    int ensureAlbumExists(const SongInfo &songInfo);
    int ensureAlbumExists(const AlbumInfo &albumInfo);
    // 检查歌曲是否存在，并插入不存在的歌曲
    int ensureSongExists(const SongInfo &songInfo);

    // 读取音乐标签
    MusicLibrary::AlbumInfo readAlbumInfo(const QString &filePath, const bool getID);
    // 每首歌的角色信息
    // Path,<Role, CharacterName, ForeignName>
    QVector<CharacterInfo> readCharacterInfo(const QString &filePath, const bool getID);
    MusicLibrary::SongInfo readSongInfo(const QString &filePath);
    // 扫描专辑信息
    QHash<QString, AlbumInfo> getAlbumsInfo(const QStringList &musicFiles);
    // 扫描人物信息
    QHash<QString, CharacterInfo> getCharactersInfo(const QStringList &musicFiles);
    // 扫描歌曲信息
    QHash<QString, SongInfo> getSongsInfo(const QStringList &musicFiles);

    // 为歌曲更新流派
    int updateGenre(const int &SongID,const QStringList &genres);
 

    void closeDatabase();

    // 分割字符串
    QStringList splitString(const QString &str);
    QStringList splitString(const QStringList &strlist);
    // 格式化年月日
    DateInfo formatDate(const QString &dateString);

signals:

};

#endif // MUSICLIBRARY_H
