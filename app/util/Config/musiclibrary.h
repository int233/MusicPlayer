#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QStandardPaths>
#include <QDir>
#include <QDebug>
#include <QSqlDatabase>
#include <QThread>

#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/tpropertymap.h>

#include "app/util/Config/configmanager.h"

#include "musicdatastruct.h"

// #include "app/util/PlayList/standardplaylist.h"
// #include "app/util/PlayList/smartplaylist.h"

class MusicLibrary : public QObject
{
    Q_OBJECT

public:
    static MusicLibrary& instance();
    ~MusicLibrary();

    QString splitPartten = "[;,/]";

    
    // struct musiclibrary
    // {
    //     // ARTIST
    //     // 艺术家
    //     QStringList artists;
    //     // ALBUM ARTIST
    //     // 专辑艺术家
    //     QStringList albumArtists;
    //     // COMPOSER
    //     // 作曲
    //     QStringList composers;
    //     // LYRICIST
    //     // 作词
    //     QStringList lyricists;
    //     // PERFORMER
    //     // 指挥
    //     QStringList performers;
    // };

    // class PlayList;
    // class StandardPlayList;
    // class SmartPlayList;



    // 返回歌曲
    // std::vector<SongItem> getAllSongs();
    // 从数据库中根据文件路径查找单个歌曲并返回信息
    SongInfo getSingleSong(const QString &path);
    // 从数据库中根据歌曲ID查找单个歌曲并返回信息
    SongInfo getSingleSong(const int &songID);
    // 从数据库中根据歌曲ID查找歌曲对应的人物
    QVector<CharacterInfo> getSongCharacters(const int &songID);

    // 加载所有PlayList的id和name
    QVector<PlayListInfo> getPlayLists();
    // 删除歌单
    bool deletePlayList(const int &playListID);
    // 插入和更新歌单
    bool insertPlayList(const QString &name, const QVector<int> &songIDs = {});
    QVector<int> loadPlayList(const QString &name);
    QVector<int> loadPlayList(const int &playListID);
    QVector<int> loadAllSongs();

    // 插入扫描结果
    bool insertScanResult(const QStringList &musicFiles);
    // 从数据库中解析插入结果
    QStringList retrieveMusicFiles() const;
    bool parseInsertResult();

    // 扫描歌曲信息
    QHash<QString, SongInfo> getSongsInfoBymusicFIles(const QStringList &musicFiles);
    QHash<int, SongInfo> getSongsInfoByID(const QVector<int> &songIDs);

private:
    explicit MusicLibrary(QObject *parent = nullptr);
    ConfigManager &configManager;

    QStringList getLibraryPaths() const;

    QSqlDatabase m_db;
    void initializeDatabase();
    void createTables();

    // 数据库连接配置
    static constexpr const char* DB_CONNECTION_NAME = "MUSIC_LIB_CONN";
    // 检查表是否存在
    bool tableExists(const QString &tableName);

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
    AlbumInfo readAlbumInfo(const QString &filePath, const bool getID);
    // 每首歌的角色信息
    // Path,<Role, CharacterName, ForeignName>
    QVector<CharacterInfo> readCharacterInfo(const QString &filePath, const bool getID);
    // 读取本地的音乐信息
    SongInfo readSongInfo(const QString &filePath);
    // 扫描专辑信息
    QHash<QString, AlbumInfo> getAlbumsInfo(const QStringList &musicFiles);
    // 扫描人物信息
    QHash<QString, CharacterInfo> getCharactersInfo(const QStringList &musicFiles);


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
