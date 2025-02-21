#include "musiclibrary.h"

MusicLibrary& MusicLibrary::instance() {
    static MusicLibrary instance;
    return instance;
}

MusicLibrary::MusicLibrary(QObject *parent)
    : QObject{parent},configManager(ConfigManager::instance())
{
    QString databasePath;
    QStringList libraryPaths = getLibraryPaths();
    databasePath = libraryPaths.first();
    qDebug() << "Library Path: " << databasePath;

    if (QSqlDatabase::contains(DB_CONNECTION_NAME)) {
        m_db = QSqlDatabase::database(DB_CONNECTION_NAME);
    } else {
        m_db = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION_NAME);
        m_db.setDatabaseName(databasePath);
    }

    if (!m_db.open()) {
        qCritical() << "Database connection error:" << m_db.lastError().text();
        return;
    }

    initializeDatabase();
}

MusicLibrary::~MusicLibrary()
{
    closeDatabase();
}

void MusicLibrary::initializeDatabase()
{
    QSqlQuery query(m_db);
    if (!query.exec("PRAGMA foreign_keys = ON;")) {
        qCritical() << "Failed to enable foreign keys:" << query.lastError().text();
        return;
    }
    createTables();
}

void MusicLibrary::closeDatabase()
{
    if (m_db.isOpen()) {
        m_db.close();
    }
    QSqlDatabase::removeDatabase(DB_CONNECTION_NAME);
}


QStringList MusicLibrary::getLibraryPaths() const {
    QString libraryFolderPath = configManager.getSettings()->value("Common/ConfigPath/").toString() + "library/";

    const QString librarySubfix = "intlibrary";
    QStringList filePaths;

    // 检查目录是否存在
    QDir libraryDir(libraryFolderPath);
    if (!libraryDir.exists()) {
        qWarning() << "Library directory does not exist:" << libraryFolderPath;
    }

    // 使用 QDirIterator 遍历目录
    QDirIterator it(libraryFolderPath, QStringList() << "*." + librarySubfix, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        filePaths.append(it.next());
    }

    if (filePaths.isEmpty()) {
        qWarning() << "No library files found in:" << libraryFolderPath;
        QString defaultPath = configManager.GetHomeLibraryPath();
        QDir dir(defaultPath);
        if (!dir.exists()) {
            dir.mkpath(".");
        }
        filePaths.append(QDir::toNativeSeparators(dir.filePath("music.intlibrary")));
    }

    return filePaths;
}

bool MusicLibrary::tableExists(const QString &tableName) {
    QSqlQuery query(m_db);
    query.exec(QString("SELECT name FROM sqlite_master WHERE type='table' AND name='%1';").arg(tableName));

    if (query.next()) {
        return true;
    } else {
        return false;
    }
}

void MusicLibrary::createTables()
{
    QSqlQuery query(m_db);

    // MusicScan表
    /*
    music_id: 扫描到的音乐id，无意义
    music_path: 音乐文件位置
    */
   query.exec(
    "CREATE TABLE IF NOT EXISTS MusicsFound ("
    "    music_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
    "    music_path     TEXT NOT NULL UNIQUE CHECK(music_path != ''),"
    "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP"
    ");"
    );

    // Albums表（专辑信息）
    /*
    album_id: 专辑id
    album_name: 专辑名
    publisher: 发布者（公司名等）
    album_score: 专辑评分，0~5
    release_time: 专辑发布时间
    total_tracks: 专辑曲目总数
    total_discs: 专辑碟数
    created_at: 添加时间
    modified_at: 修改时间
    */
    query.exec(
        "CREATE TABLE IF NOT EXISTS Albums ("
        "    album_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    album_name     TEXT NOT NULL CHECK(album_name != ''),"
        "    album_first_artist     TEXT NOT NULL CHECK(album_first_artist != ''),"
        "    publisher      TEXT,"
        "    album_score    REAL CHECK(album_score BETWEEN 0 AND 5),"
        "    album_version  TEXT,"
        "    original_release_year   INTEGER,"
        "    original_release_month   INTEGER,"
        "    original_release_day   INTEGER,"
        "    original_release_level   INTEGER,"
        "    album_release_year   INTEGER,"
        "    album_release_month   INTEGER,"
        "    album_release_day   INTEGER,"
        "    album_release_level   INTEGER,"
        "    country        TEXT,"
        "    genres          TEXT,"
        "    total_discs    INTEGER,"
        "    cover_path     TEXT,"
        "    languages     TEXT,"
        "    live_event     TEXT,"
        "    live_event_count TEXT,"
        "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    modified_at    DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");"
        );
    query.exec(
        "CREATE TABLE IF NOT EXISTS Tracks ("
        "    track_id       INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    album_id     INTEGER CHECK(album_id > 0),"
        "    title          TEXT,"
        "    track_number   INTEGER CHECK(track_number > 0),"
        "    media_number   INTEGER CHECK(media_number > 0),"
        "    genres          TEXT,"
        "    languages     TEXT,"
        "    release_time   DATETIME,"
        "    start_time     DATETIME,"
        "    end_time       DATETIME,"
        "    cover_path     TEXT,"
        "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP,"
        ");"
        );
    // Characters表（角色信息）
    /*
    character_id
    character_name: 姓名
    character_foreign_name: 外文名
    portrait_path: 头像路径

    扫描音乐时，通过姓名和外文名匹配，若存在则使用已有角色，若不存在则插入新角色
    若需插入相同姓名的人，通过overlap_id区分，默认为0
    */
    query.exec(
        "CREATE TABLE IF NOT EXISTS Characters ("
        "    character_id             INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    character_name           TEXT,"
        "    character_foreign_name   TEXT,"
        "    portrait_path            TEXT,"
        "    overlap_id               INTEGER DEFAULT 0,"
        "    created_at               DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    modified_at              DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");"
        );

    // Songs表（歌曲核心信息）
    /*
    song_id
    title: 歌曲名称
    duration_ms: 歌曲时长（毫秒）
    file_size: 文件大小（KB）
    file_path: 文件位置
    album_id: 专辑
    track_number: 曲目次序
    disc_number: 碟号
    is_favorite: 是否喜爱
    is_live: 是否现场版
    is_single: 是否单曲
    live_event: 现场演出名称
    live_event_count: 现场演出场次
    is_cover: 是否翻唱
    is_mashup: 是否串烧
    release_time: 发布时间
    */
    query.exec(
        "CREATE TABLE IF NOT EXISTS Songs ("
        "    song_id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    title          TEXT NOT NULL CHECK(title != ''),"
        "    durations    INTEGER CHECK(durations > 0),"
        "    first_artist   TEXT NOT NULL CHECK(first_artist != ''),"
        "    file_size      INTEGER CHECK(file_size > 0),"
        "    file_path      TEXT NOT NULL UNIQUE CHECK(file_path != ''),"
        "    album_id       INTEGER REFERENCES Albums(album_id) ON DELETE SET NULL,"
        "    track_number   INTEGER CHECK(track_number >= 0),"
        "    disc_number    INTEGER CHECK(disc_number >= 0),"
        "    total_track_number   INTEGER DEFAULT 1 CHECK(track_number >= 0),"
        "    total_disc_number    INTEGER DEFAULT 1 CHECK(disc_number >= 0),"
        "    is_favorite    BOOLEAN DEFAULT 0,"
        "    is_live        BOOLEAN DEFAULT 0,"
        "    is_single      BOOLEAN DEFAULT 0,"
        "    live_event     TEXT,"
        "    live_event_count TEXT,"
        "    is_cover       BOOLEAN DEFAULT 0,"
        "    is_mashup      BOOLEAN DEFAULT 0,"
        "    ori_release_year   INTEGER,"
        "    ori_release_month   INTEGER,"
        "    ori_release_day   INTEGER,"
        "    ori_release_level   INTEGER,"
        "    release_year   INTEGER,"
        "    release_month   INTEGER,"
        "    release_day   INTEGER,"
        "    release_level   INTEGER,"
        "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    modified_at     DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");"
        );

    // 多对多关系表
    // SongCharacters

    // 角色列表
    if (!tableExists("RoleCategories")) {
        query.exec(
            "CREATE TABLE RoleCategories ("
            "    id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "    category TEXT NOT NULL,"
            "    role TEXT NOT NULL"
            ");"
        );
        if (query.lastError().isValid()) {
            qWarning() << "Failed to create table RoleCategories:" << query.lastError();
            return;
        }
        qDebug() << "Existing resources:" << QDir(":/").entryList();
        QFile file(":/resource/rolecategories.txt");
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qWarning() << "Could not open rolecategories.txt:" << file.errorString();
            return;
        }
        QTextStream in(&file);
        QSqlQuery query(m_db);
        while (!in.atEnd()) {
            QString line = in.readLine();
            QStringList fields = line.split("\t");
    
            if (fields.size() != 2) {
                qWarning() << "Invalid line format in rolecategories.txt:" << line;
                continue;
            }
            QString category = fields[0];
            QString role = fields[1];
            query.prepare("INSERT INTO RoleCategories (category, role) VALUES (:category, :role)");
            query.bindValue(":category", category);
            query.bindValue(":role", role);
            if (!query.exec()) {
                qWarning() << "Failed to insert record into RoleCategories:" << query.lastError();
            }
        }
    } else {
        qDebug() << "Table RoleCategories already exists.";
    }
    
    // 歌曲与角色对应
    query.exec(
        "CREATE TABLE IF NOT EXISTS SongCharacters ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    character_id   INTEGER REFERENCES Characters(character_id) ON DELETE CASCADE,"
        "    role           TEXT,"
        "    PRIMARY KEY (song_id, character_id, role)"
        ");"
        );

    // SongMetadata
    // Genre
    query.exec(
        "CREATE TABLE IF NOT EXISTS SongGenres ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    genre          TEXT,"
        "    PRIMARY KEY (song_id, genre)"
        ");"
        );

    // Language
    query.exec(
        "CREATE TABLE IF NOT EXISTS SongLanguages ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    language          TEXT,"
        "    PRIMARY KEY (song_id, language)"
        ");"
        );

    // 歌单
    // 歌单信息
    query.exec(
        "CREATE TABLE IF NOT EXISTS PlayList ("
        "    playlist_id        INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    name          TEXT UNIQUE"
        ");"
        );
    // 初始的歌单信息
    if (!query.exec(
        "INSERT OR IGNORE INTO PlayList (playlist_id, name) VALUES "
        "(0, 'AllSongs'), (1, 'TempSongs');"
    )) {
        qDebug() << "Insert data error:" << query.lastError();
    }
    

    // 歌单和歌曲的关系
    query.exec(
        "CREATE TABLE IF NOT EXISTS SongsPlayList ("
        "    playlist_id       INTEGER REFERENCES PlayList(playlist_id) ON DELETE CASCADE,"
        "    song_id          INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE"
        ");"
        );

    // 其他索引
    query.exec("CREATE INDEX IF NOT EXISTS idx_songs_album ON Songs(album_id);");
    query.exec("CREATE INDEX IF NOT EXISTS idx_song_characters ON SongCharacters(character_id);");
}


// 实现专辑检查与插入逻辑
int MusicLibrary::ensureAlbumExists(const SongInfo &songInfo)
{
    // 如果已指定有效专辑ID且存在
    if (songInfo.albumID > 0) {
        QSqlQuery checkQuery(m_db);
        checkQuery.prepare("SELECT 1 FROM Albums WHERE album_id = ?");
        checkQuery.addBindValue(songInfo.albumID);
        if (checkQuery.exec() && checkQuery.next()) {
            return songInfo.albumID;
        }
    }

    // 需要创建新专辑的检查
    if (songInfo.albumID == -1) {
        return -1; // 表示不使用专辑
    }

    // 插入新专辑
    QSqlQuery query(m_db);
    query.prepare(
        "INSERT INTO Albums ("
        "  album_name, publisher, album_version, release_time, total_tracks, total_discs"
        ") VALUES ("
        "  :name, :publisher, :version, :releaseTime, :tracks, :discs"
        ")"
        );

    // query.bindValue(":name", songInfo.albumName);
    // query.bindValue(":publisher", songInfo.publisher);
    // query.bindValue(":version", songInfo.albumVersion);
    // query.bindValue(":releaseTime", songInfo.albumReleaseTime);
    query.bindValue(":tracks", songInfo.totalTracks > 0 ? songInfo.totalTracks : QVariant());
    query.bindValue(":discs", songInfo.totalDiscs > 0 ? songInfo.totalDiscs : QVariant());

    if (!query.exec()) {
        throw std::runtime_error("Insert album failed: " + query.lastError().text().toStdString());
    }

    return query.lastInsertId().toInt();
}

int MusicLibrary::ensureAlbumExists(const AlbumInfo &albumInfo)
{

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << "Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    // 专辑名
    QString albumName = albumInfo.albumName;

    // 专辑演出者
    QString albumFirstArtist = albumInfo.albumArtists.isEmpty() ? "Unknown" : albumInfo.albumArtists.first();
    

    // 专辑评分
    int albumScore = albumInfo.albumScore;
    

    // 专辑版本
    QString albumVersion = albumInfo.albumVersion;
    

    // 发行方，转换为以逗号分隔的字符串
    QString publisherStr = albumInfo.publishers.join(", ");
    

    // 流派和语言

    // 专辑发布日期，分别以int的形式存储年月日，使用level区分
    int albumReleaseYear = albumInfo.albumReleaseTime.date.year();
    int albumReleaseMonth = albumInfo.albumReleaseTime.date.month();
    int albumReleaseDay = albumInfo.albumReleaseTime.date.day();
    int albumReleaseLevel = albumInfo.albumReleaseTime.level;
    

    // 国家
    QString countryStr = albumInfo.country;
    

    // 碟数
    int totalDiscs = albumInfo.totalDiscs;
    

    // 演出信息
    QString liveEvent = albumInfo.liveEvent;
    QString liveEventCount = albumInfo.liveEventCount;
    

    // 检查专辑是否存在
    query.prepare("SELECT album_id FROM Albums WHERE "
        "album_name = :albumName AND "
        "album_first_artist = :albumFirstArtist AND "
        "album_release_year = :albumReleaseYear");
    query.bindValue(":albumName", albumName);
    query.bindValue(":albumFirstArtist", albumFirstArtist);
    query.bindValue(":albumReleaseYear", albumReleaseYear);

    if (!query.exec()) {
        qWarning() << "Check album existence failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }
    qDebug() << "Executing query:" << query.lastQuery();
    qDebug() << "Bound values:" << query.boundValues();

    if (query.next()) {
        int existingAlbumID = query.value(0).toInt();
        m_db.commit();
        qDebug() << "Album already exists:" << existingAlbumID;
        return existingAlbumID;
    }

    qDebug() << "ensureAlbumExists --  Name:" << albumName
         << ", First Artist:" << albumFirstArtist
         << ", Score:" << albumScore
         << ", Version:" << albumVersion
         << ", Publisher:" << publisherStr
         << ", Release Time:" << albumReleaseYear << "-" << albumReleaseMonth << "-" << albumReleaseDay << " Level:" << albumReleaseLevel
         << ", Country:" << countryStr
         << ", Total Discs:" << totalDiscs
         << ", Live Event:" << liveEvent << " Count:" << liveEventCount;

    // 添加新专辑
    query.prepare("INSERT INTO Albums ("
        "album_name, album_first_artist, publisher, album_score, album_version, "
        "original_release_year, original_release_month, original_release_day, original_release_level, "
        "album_release_year, album_release_month, album_release_day, album_release_level, "
        "country, genres, total_discs, "
        "cover_path, languages, live_event, live_event_count)"
        "VALUES ("
        ":albumName, :albumFirstArtist, :publisher, :albumScore, :albumVersion, "
        ":originalReleaseYear, :originalReleaseMonth, :originalReleaseDay, :originalReleaseLevel, "
        ":albumReleaseYear, :albumReleaseMonth, :albumReleaseDay, :albumReleaseLevel, "
        ":country, :genres, :totalDiscs, "
        ":coverPath, :languages, :liveEvent, :liveEventCount)");
    query.bindValue(":albumName", albumName);
    query.bindValue(":albumFirstArtist", albumFirstArtist);
    query.bindValue(":publisher", publisherStr);
    query.bindValue(":albumScore", albumScore);
    query.bindValue(":albumVersion", albumVersion);
    query.bindValue(":originalReleaseYear", albumInfo.originalReleaseTime.date.year());
    query.bindValue(":originalReleaseMonth", albumInfo.originalReleaseTime.date.month());
    query.bindValue(":originalReleaseDay", albumInfo.originalReleaseTime.date.day());
    query.bindValue(":originalReleaseLevel", albumInfo.originalReleaseTime.level);
    query.bindValue(":albumReleaseYear", albumReleaseYear);
    query.bindValue(":albumReleaseMonth", albumReleaseMonth);
    query.bindValue(":albumReleaseDay", albumReleaseDay);
    query.bindValue(":albumReleaseLevel", albumReleaseLevel);
    query.bindValue(":country", countryStr);
    query.bindValue(":genres", albumInfo.genres.join(", "));
    query.bindValue(":totalDiscs", totalDiscs);
    query.bindValue(":coverPath", albumInfo.albumCoverPath);
    query.bindValue(":languages", albumInfo.languages.join(", "));
    query.bindValue(":liveEvent", liveEvent);
    query.bindValue(":liveEventCount", liveEventCount);
    
    if (!query.exec()) {
        if (query.lastError().nativeErrorCode() == "2067") { // SQLite 的唯一性错误代码
            qDebug() << "Album already exists, skipping insertion.";
            m_db.rollback();
            return -1;
        } else {
            qWarning() << "Insert album failed:" << query.lastError();
            m_db.rollback();
            return -1;
        }
    }
    

    int newAlbumID = query.lastInsertId().toInt();
    qDebug() << "New album inserted:" << newAlbumID;

    if (!m_db.commit()) {
        qWarning() << "Commit transaction failed:" << m_db.lastError();
        return -1;
    }

    return newAlbumID;
} 

int MusicLibrary::updateGenre(const int &SongID,const QStringList &genres){
    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return -1;
    }

    if (!m_db.transaction()) {
        qWarning() << "Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    QSqlQuery query(m_db);

    // 删除原有流派
    query.prepare("DELETE FROM Genre WHERE song_id = ?");
    query.addBindValue(SongID);
    if (!query.exec()) {
        qWarning() << "Failed to delete old genres:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    // 插入新流派
    query.prepare("INSERT INTO Genre (song_id, genre) VALUES (?, ?)");
    query.addBindValue(SongID);
    foreach (const QString &genre, genres) {
        query.addBindValue(genre);
        if (!query.exec()) {
            qWarning() << "Failed to insert genre:" << query.lastError();
            m_db.rollback();
            return -1;
        }
    }

    if (!m_db.commit()) {
        qWarning() << "Failed to commit transaction:" << m_db.lastError();
        return -1;
    }

    return 0;

}

int MusicLibrary::ensureCharacterExists(const QString &characterName, const QString &foreignName)
{

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << "Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    // 检查输入有效性
    if (characterName.isEmpty() && foreignName.isEmpty()) {
        throw std::runtime_error("Character name and foreign name cannot both be empty");
    }

    // 检查角色是否存在
    if (characterName.isEmpty() && !foreignName.isEmpty()){
        query.prepare("SELECT character_id FROM Characters WHERE "
            "character_name ISNULL and character_foreign_name = :foreignName AND "
            "overlap_id = 0"
        );
        // query.bindValue(":characterName", characterName.isEmpty() ? QVariant() : characterName);
        query.bindValue(":foreignName", foreignName.isEmpty() ? QVariant() : foreignName);
    } else if (!characterName.isEmpty() && foreignName.isEmpty() ){
        query.prepare("SELECT character_id FROM Characters WHERE "
            "character_name  = :characterName and character_foreign_name ISNULL AND "
            "overlap_id = 0"
        );
        query.bindValue(":characterName", characterName.isEmpty() ? QVariant() : characterName);
        // query.bindValue(":foreignName", foreignName.isEmpty() ? QVariant() : foreignName);
    } else if (!characterName.isEmpty() && !foreignName.isEmpty() ){
        query.prepare("SELECT character_id FROM Characters WHERE "
            "character_name = :characterName AND "
            "character_foreign_name = :foreignName AND "
            "overlap_id = 0");
            query.bindValue(":characterName", characterName.isEmpty() ? QVariant() : characterName);
            query.bindValue(":foreignName", foreignName.isEmpty() ? QVariant() : foreignName);
    }

    qDebug() << "Executing query:" << query.lastQuery();
    qDebug() << "Bound values:" << query.boundValues();
    
    if (!query.exec()) {
        qWarning() << "Check character existence failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (query.next()) {
        int existingCharacterId = query.value(0).toInt();
        m_db.commit();
        return existingCharacterId;
    }

    // 插入新人名
    query.prepare("INSERT INTO Characters (character_name, character_foreign_name) "
                  "VALUES (:characterName, :foreignName)");
    query.bindValue(":characterName", characterName);
    query.bindValue(":foreignName", foreignName);

    if (!query.exec()) {
        qWarning() << "Insert character failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    // 获取新插入的 character_id
    int newCharacterId = query.lastInsertId().toInt();
    qDebug() << "New character inserted with ID:" << newCharacterId;

    // 提交事务
    if (!m_db.commit()) {
        qWarning() << "Commit transaction failed:" << m_db.lastError();
        return -1;
    }

    return newCharacterId;
}

bool MusicLibrary::insertScanResult(const QStringList &musicFiles){

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    QSqlQuery query(m_db);
    
    // 开启事务（大幅提升插入性能）
    if (!m_db.transaction()) {
        qCritical() << "Failed to start transaction:" << m_db.lastError().text();
        return false;
    }

    // 使用预处理语句（防止SQL注入，提升性能）
    query.prepare("INSERT OR IGNORE INTO MusicsFound (music_path) VALUES (:path)");
    
    for (const QString &filePath : musicFiles) {
        query.bindValue(":path", filePath);
        
        if (!query.exec()) {
            qCritical() << "Failed to insert file:" << filePath 
                       << "Error:" << query.lastError().text();
            m_db.rollback();  // 回滚事务
            return false;
        }
    }

    // 提交事务
    if (!m_db.commit()) {
        qCritical() << "Commit failed:" << m_db.lastError().text();
        m_db.rollback();
        return false;
    }

    qDebug() << "Inserted" << musicFiles.size() << "records (ignored duplicates)";
    return true;
}

QStringList MusicLibrary::retrieveMusicFiles() const {
    QStringList musicFiles;

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return musicFiles;
    }

    QSqlQuery query(m_db);
    const QString sql = "SELECT music_path FROM MusicsFound";  // 确保表名与插入时一致
    
    if (!query.exec(sql)) {
        qCritical() << "Failed to execute query:" << query.lastError().text();
        return musicFiles;
    }

    while (query.next()) {
        const QString path = query.value("music_path").toString();
        if (!path.isEmpty()) {  // 可选：过滤空路径
            musicFiles.append(path);
        }
    }

    qDebug() << "Retrieved" << musicFiles.size() << "music files from database";
    return musicFiles;
}

bool MusicLibrary::parseInsertResult()
{
    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    QStringList musicFiles = retrieveMusicFiles();
    if (musicFiles.isEmpty()) {
        qWarning() << "No music files found in database";
        return false;
    }

    // 读取所有的专辑信息
    QHash albumDatabase = getAlbumsInfo(musicFiles);
    
    // 读取所有的人物信息
    QHash characterDatabase = getCharactersInfo(musicFiles);

    // 向数据库中插入专辑和人物
    foreach (const AlbumInfo &album, albumDatabase) {
        int albumID = ensureAlbumExists(album);
        if (albumID == -1) {
            qWarning() << "Failed to insert album:" << album.albumName;
        }
    }

    foreach (const CharacterInfo &character, characterDatabase) {
        int characterID = ensureCharacterExists(character.characterName, character.foreignName);
        if (characterID == -1) {
            qWarning() << "Failed to insert character:" << character.characterName;
        }
    }


    // 读取所有的歌曲信息，并替换专辑和人物信息为ID
    QHash songDatabase = getSongsInfoBymusicFIles(musicFiles);

    // 向数据库中插入歌曲信息
    foreach (const SongInfo &song, songDatabase) {
        int songID = ensureSongExists(song);
        if (songID == -1) {
            qWarning() << "Failed to insert song:" << song.title;
        }
    }

    return true;
}

int MusicLibrary::ensureSongExists(const SongInfo &songInfo){

    QString debugTitle = "ensureSongExists";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << debugTitle << " - Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    qDebug() << debugTitle << " - Ensure song exists:" << songInfo.title;

    // 歌曲名
    QString title = songInfo.title;

    // first artist
    QString first_artist = songInfo.artists.isEmpty() ? "Unknown" : songInfo.artists.first();

    // 时长
    int durations = songInfo.durations;

    // 文件大小
    int file_size = songInfo.fileSize;

    // 文件路径
    QString file_path = songInfo.filePath;

    // 专辑ID
    int album_id = songInfo.albumID;

    // 曲目次序
    int track_number = songInfo.trackNumber;
    int disc_number = songInfo.discNumber;
    int total_track_number = songInfo.totalTracks;
    int total_disc_number = songInfo.totalDiscs;

    // 是否喜爱
    bool is_favorite = songInfo.isFavorite;

    // 是否现场版
    bool is_live = songInfo.isLive;

    // 是否单曲
    bool is_single = songInfo.isSingle;

    // 是否翻唱
    bool is_cover = songInfo.isCover;

    // 是否串烧
    bool is_mashup = songInfo.isMashup;

    // 现场演出
    QString live_event = songInfo.liveEvent;
    QString live_event_count = songInfo.liveEventCount;

    // 发布时间
    // 原始发布时间
    int ori_release_year = songInfo.originalReleaseDate.date.year();
    int ori_release_month = songInfo.originalReleaseDate.date.month();
    int ori_release_day = songInfo.originalReleaseDate.date.day();
    int ori_release_level = songInfo.originalReleaseDate.level;

    // 发布时间
    int release_year = songInfo.releaseDate.date.year();
    int release_month = songInfo.releaseDate.date.month();
    int release_day = songInfo.releaseDate.date.day();
    int release_level = songInfo.releaseDate.level;

    // 检查歌曲是否存在
    query.prepare("SELECT song_id FROM Songs WHERE "
        "file_path = :filePath");
    query.bindValue(":filePath", file_path);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check song existence failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (query.next()) {
        int existingSongID = query.value(0).toInt();
        m_db.commit();
        qDebug() << debugTitle << " - Song already exists:" << existingSongID;
        return existingSongID;
    }

    // 添加新歌曲

    query.prepare("INSERT INTO Songs ("
        "title, durations, first_artist, file_size, file_path, album_id, track_number, disc_number, "
        "total_track_number, total_disc_number, is_favorite, is_live, is_single, live_event,"
        "live_event_count, is_cover, is_mashup, ori_release_year, ori_release_month, "
        "ori_release_day, ori_release_level, release_year, release_month, release_day, release_level"
        ") VALUES ("
        ":title, :durations, :first_artist, :file_size, :file_path, :album_id, :track_number, :disc_number, "
        ":total_track_number, :total_disc_number, :is_favorite, :is_live, :is_single, :live_event,"
        ":live_event_count, :is_cover, :is_mashup, :ori_release_year, :ori_release_month, "
        ":ori_release_day, :ori_release_level, :release_year, :release_month, :release_day, :release_level"
        ")");
    query.bindValue(":title", title);
    query.bindValue(":durations", durations);
    query.bindValue(":first_artist", first_artist);
    query.bindValue(":file_size", file_size);
    query.bindValue(":file_path", file_path);
    query.bindValue(":album_id", album_id);
    query.bindValue(":track_number", track_number);
    query.bindValue(":disc_number", disc_number);
    query.bindValue(":total_track_number", total_track_number);
    query.bindValue(":total_disc_number", total_disc_number);
    query.bindValue(":is_favorite", is_favorite);
    query.bindValue(":is_live", is_live);
    query.bindValue(":is_single", is_single);
    query.bindValue(":live_event", live_event); 
    query.bindValue(":live_event_count", live_event_count);
    query.bindValue(":is_cover", is_cover);
    query.bindValue(":is_mashup", is_mashup);
    query.bindValue(":ori_release_year", ori_release_year);
    query.bindValue(":ori_release_month", ori_release_month);
    query.bindValue(":ori_release_day", ori_release_day);
    query.bindValue(":ori_release_level", ori_release_level);
    query.bindValue(":release_year", release_year); 
    query.bindValue(":release_month", release_month);
    query.bindValue(":release_day", release_day);
    query.bindValue(":release_level", release_level);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        if (query.lastError().nativeErrorCode() == "2067") { // SQLite 的唯一性错误代码
            qDebug() << debugTitle << " - Song already exists, skipping insertion.";
            m_db.rollback();
            return -1;
        } else {
            qWarning() << debugTitle << " - Insert song failed:" << query.lastError();
            m_db.rollback();
            return -1;
        }
    }

    int newSongID = query.lastInsertId().toInt();
    qDebug() << debugTitle << " - New song inserted:" << newSongID;

    if (!m_db.commit()) {
        qWarning() << debugTitle << " - Commit transaction failed:" << m_db.lastError();
        return -1;
    }

    // 处理角色关联
    foreach (const CharacterInfo &character, songInfo.characters) {
        int characterID = character.characterID;
        QString role = character.role;
        int res = pairSongCharacter(newSongID, characterID, role);
    }

    // 处理流派
    foreach (const QString &genre, songInfo.genres) {
        int res = pairSongGenre(newSongID, genre);
    }

    // 处理语言
    foreach (const QString &language, songInfo.languages) {
        int res = pairSongLanguage(newSongID, language);
    }
    return newSongID;
}

int MusicLibrary::pairSongLanguage(const int &songID, const QString &language){

    QString debugTitle = "pairSongLanguage";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << debugTitle << " - Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    query.prepare("INSERT INTO SongLanguages (song_id, language) "
                  "VALUES (:songID, :language)");
    query.bindValue(":songID", songID);
    query.bindValue(":language", language);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Failed to insert song-language:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit()) {
        qWarning() << debugTitle << " - Failed to commit transaction:" << m_db.lastError();
        return -1;
    }

    return 0;

}

int MusicLibrary::pairSongGenre(const int &songID, const QString &genre){

    QString debugTitle = "pairSongGenre";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << debugTitle << " - Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    query.prepare("INSERT INTO SongGenres (song_id, genre) "
                  "VALUES (:songID, :genre)");
    query.bindValue(":songID", songID);
    query.bindValue(":genre", genre);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Failed to insert song-genre:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit()) {
        qWarning() << debugTitle << " - Failed to commit transaction:" << m_db.lastError();
        return -1;
    }

    return 0;

}

int MusicLibrary::pairSongCharacter(const int &songID, const int &characterID, const QString &role){

    QString debugTitle = "pairSongCharacter";

    if (!m_db.isOpen()) {
        qCritical() <<  debugTitle << " - Database is not open";
        return -1;
    }

    QSqlQuery query(m_db);

    if (!m_db.transaction()) {
        qWarning() << debugTitle << " - Failed to start transaction:" << m_db.lastError();
        return -1;
    }

    query.prepare("INSERT INTO SongCharacters (song_id, character_id, role) "
                  "VALUES (?, ?, ?)");
    query.bindValue(0,songID); 
    query.bindValue(1,characterID);
    query.bindValue(2,role);
    
    qDebug() << debugTitle << " - Raw SQL:" << query.lastQuery().toUtf8().constData();
    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Failed to insert song-character:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit()) {
        qWarning() <<  debugTitle << " - Failed to commit transaction:" << m_db.lastError();
        return -1;
    }

    return 0;

}

// 从数据库中根据文件路径查找单个歌曲并返回信息
SongInfo MusicLibrary::getSingleSong(const QString &path){

    QString debugTitle = "getSingleSong";

    if (!m_db.isOpen()) {
        qCritical() <<  debugTitle << " - Database is not open";
        return SongInfo();
    }

    QSqlQuery query(m_db);

    query.prepare("SELECT song_id FROM Songs WHERE "
        "file_path = :filePath");
    query.bindValue(":filePath", path);

    qDebug() <<  debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() <<  debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() <<  debugTitle << " - Check song existence failed:" << query.lastError();
        m_db.rollback();
        return SongInfo();
    }
    qDebug() <<  debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() <<  debugTitle << " - Bound values:" << query.boundValues();

    if (query.next()) {
        int existingSongID = query.value(0).toInt();
        m_db.commit();
        qDebug() <<  debugTitle << " - Song already exists:" << existingSongID;
        SongInfo songInfo = getSingleSong(existingSongID);
        return songInfo;
    } else {
        return SongInfo();
    }
}

// 从数据库中根据歌曲ID查找单个歌曲并返回信息
SongInfo MusicLibrary::getSingleSong(const int &songID){

    QString debugTitle = "getSingleSong";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return SongInfo();
    }

    QSqlQuery query(m_db);

    query.prepare("SELECT "
        "song_id, title, durations, first_artist, file_size, file_path, album_id, "
        "track_number, disc_number, total_track_number, total_disc_number, "
        "is_favorite, is_live, is_single, live_event, live_event_count, is_cover, is_mashup, "
        "ori_release_year, ori_release_month,ori_release_day,ori_release_level, "
        "release_year, release_month, release_day, release_level, created_at, modified_at "
        "FROM Songs WHERE song_id = :songID");
    query.bindValue(":songID", songID);
    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check song existence failed:" << query.lastError();
        m_db.rollback();
        return SongInfo();
    }
    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.next()){
        qWarning() << debugTitle << " - No song found with ID: " << songID;
        return SongInfo();
    }

    SongInfo songInfo;
    songInfo.songID = songID;
    songInfo.title = query.value("title").toString();
    songInfo.durations = query.value("durations").toInt();
    songInfo.fileSize = query.value("file_size").toInt();
    songInfo.filePath = query.value("file_path").toString();
    songInfo.albumID = query.value("album_id").toInt();
    songInfo.trackNumber = query.value("track_number").toInt();
    songInfo.discNumber = query.value("disc_number").toInt();
    songInfo.totalTracks = query.value("total_track_number").toInt();
    songInfo.totalDiscs = query.value("total_disc_number").toInt();
    songInfo.isFavorite = query.value("is_favorite").toBool();
    songInfo.isLive = query.value("is_live").toBool();
    songInfo.isSingle = query.value("is_single").toBool();
    songInfo.liveEvent = query.value("live_event").toString();
    songInfo.liveEventCount = query.value("live_event_count").toString();
    songInfo.isCover = query.value("is_cover").toBool();
    songInfo.isMashup = query.value("is_mashup").toBool();

    // 处理原始发布日期
    songInfo.originalReleaseDate.date = QDate(query.value("ori_release_year").toInt(), query.value("ori_release_month").toInt(), query.value("ori_release_day").toInt());
    songInfo.originalReleaseDate.level = query.value("ori_release_level").toInt();

    // 处理发布日期
    songInfo.releaseDate.date = QDate(query.value("release_year").toInt(), query.value("release_month").toInt(), query.value("release_day").toInt());
    songInfo.releaseDate.level = query.value("release_level").toInt();

    // 人物
    songInfo.characters = getSongCharacters(songID);
    songInfo.artists = songInfo.getCharacters("Primary Artist");

    // 流派
    songInfo.genres = {};
    qDebug() << debugTitle << " - genres";
    // 语言
    songInfo.languages = {};
    qDebug() << debugTitle << " - languages";

    return songInfo;
}

QVector<CharacterInfo> MusicLibrary::getSongCharacters(const int &songID){

    QString debugTitle = "getSongCharacters";
    QVector<CharacterInfo> characters;

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return QVector<CharacterInfo>();
    }

    QSqlQuery query(m_db);

    query.prepare(
        "SELECT "
        "c.character_id, "
        "c.character_name, " 
        "c.character_foreign_name, "  
        "sc.role "  
        "FROM SongCharacters sc "
        "JOIN Characters c ON sc.character_id = c.character_id "
        "WHERE sc.song_id = :songID"
    );
    query.bindValue(":songID", songID);
    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();
    
    if (!query.exec()) {
        qCritical() << debugTitle << " - Query failed:" << query.lastError().text();
        return QVector<CharacterInfo>();
    }

    while (query.next()){
        int id = query.value("character_id").toInt();
        // qDebug() << debugTitle << " - id: " << id;
        QString characterName = query.value("character_foreign_name").toString();
        // qDebug() << debugTitle << " - character_foreign_name: " << characterName;
        QString foreignName = query.value("character_name").toString();
        // qDebug() << debugTitle << " - character_name: " << foreignName;
        QString role = query.value("role").toString();
        // qDebug() << debugTitle << " - role: " << role;

        CharacterInfo info(role, characterName, foreignName);
        info.characterID = id;

        characters.append(info);
    }
    return characters;
}


QHash<QString, SongInfo> MusicLibrary::getSongsInfoBymusicFIles(const QStringList &musicFiles) {

    QString debugTitle = "getSongsInfoBymusicFIles";
    QHash<QString, SongInfo> songDatabase;
    foreach(const QString& filePath, musicFiles){
        SongInfo song = readSongInfo(filePath);
        // 生成唯一键（歌名、专辑ID、首艺术家）
        // QString key = song.title + "|" + (song.albumID == -1 ? "0" : song.albumID) + "|" + song.artists.first();
        QString key = song.filePath;
        // 若存在重复曲目，抛出异常
        if(songDatabase.contains(key)) {
            qWarning() << debugTitle << " - Duplicate song found:" << song.title << song.albumID << song.artists.first() << song.filePath;
        } else {
            songDatabase.insert(key, song);
        }
    }

    // 打印出所有Song的信息
    foreach(const SongInfo& song, songDatabase){
        qDebug() << debugTitle << " - Title: " << song.title << ", album ID: "<< song.albumID << ", 专辑演出者: " << song.artists.first() << ", Path: " <<song.filePath;
    }

    return songDatabase;
}

QHash<int, SongInfo> MusicLibrary::getSongsInfoByID(const QVector<int> &songIDs) {

    QString debugTitle = "getSongsInfoByID";
    QHash<int, SongInfo> songDatabase;

    foreach(const int& songID, songIDs){
        qDebug() << debugTitle << " - Next: getSingleSong(songID), songID: " << songID;
        SongInfo song = getSingleSong(songID);
        qDebug() << debugTitle << " - Last: getSingleSong(songID), songID: " << songID;
        // songID为key
        int key = song.songID;
        qDebug() << debugTitle << " - songID: " << song.songID;
        // 若存在重复曲目，抛出异常
        if(songDatabase.contains(key)) {
            qWarning() << debugTitle << " - Duplicate song found:" << song.title << song.albumID << song.artists.first() << song.filePath;
        } else {
            songDatabase.insert(key, song);
        }
    }

    // 打印出所有Song的信息
    foreach(const SongInfo& song, songDatabase){
        qDebug() << debugTitle << " - Title: " << song.title << ", album ID: "<< song.albumID << ", 专辑演出者: " << song.artists.first() << ", Path: " <<song.filePath;
    }

    return songDatabase;
}

QVector<CharacterInfo> MusicLibrary::readCharacterInfo(const QString &filePath, const bool getID){

    QString debugTitle = "readCharacterInfo";
    QVector<CharacterInfo> characters;

    TagLib::FileRef file(filePath.toStdString().c_str());

    if(file.isNull() || !file.tag()) {
        qWarning() << debugTitle << " - 无法读取文件标签:" << filePath;
        return characters;
    }

    TagLib::Tag* tag = file.tag();
    TagLib::PropertyMap props = file.file()->properties();

    // 演出者
    QStringList artists = splitString(QString::fromStdString(tag->artist().toCString(true)));
    for (const QString &artist : artists) {
        characters.append(CharacterInfo("Primary Artist", artist, ""));
    }
    // 专辑演出者
    QStringList albumArtists = splitString(QString::fromStdString(props["ALBUMARTIST"].toString().toCString(true)));
    for (const QString &albumArtist : albumArtists) {
        characters.append(CharacterInfo("Album Artist", albumArtist, ""));
    }
    // 作曲
    QStringList composers = splitString(QString::fromStdString(props["COMPOSER"].toString().toCString(true)));
    for (const QString &composer : composers) {
        characters.append(CharacterInfo("Composer", composer, ""));
    }
    // 作词
    QStringList lyricists = splitString(QString::fromStdString(props["LYRICIST"].toString().toCString(true)));
    for (const QString &lyricist : lyricists) {
        characters.append(CharacterInfo("Lyricist", lyricist, ""));
    }
    // 指挥
    QStringList conductors = splitString(QString::fromStdString(props["CONDUCTOR"].toString().toCString(true)));
    for (const QString &conductor : conductors) {
        characters.append(CharacterInfo("Conductor", conductor, ""));
    }

    // 尝试获取ID
    if (getID) {
        for (CharacterInfo &character : characters) {
            int characterID = ensureCharacterExists(character.characterName, character.foreignName);
            if (characterID > 0) {
                character.characterID = characterID;
            }
        }
    }
    
    return characters;
}

QHash<QString, CharacterInfo> MusicLibrary::getCharactersInfo(const QStringList &musicFiles) {

    QString debugTitle = "getCharactersInfo";
    QHash<QString, CharacterInfo> characterDatabase;

    foreach(const QString& filePath, musicFiles){
        QVector<CharacterInfo> characters = readCharacterInfo(filePath, false);
        for (const CharacterInfo& character : characters) {
            QString key = filePath + "|" + character.characterName + "|" + character.role;
            characterDatabase.insert(key, character);
        }
    }

    // 打印出所有characters的信息
    foreach(const CharacterInfo& character, characterDatabase){
        qDebug() << debugTitle << " - Name: " << character.characterName <<  "role: " << character.role;
    }

    return characterDatabase;
}


AlbumInfo MusicLibrary::readAlbumInfo(const QString &filePath, const bool getID = false){

    QString debugTitle = "readAlbumInfo";
    AlbumInfo album;

    TagLib::FileRef file(filePath.toStdString().c_str());

    if(file.isNull() || !file.tag()) {
        qWarning() << debugTitle << " - 无法读取文件标签:" << filePath;
        return album;
    }

    TagLib::Tag* tag = file.tag();
    TagLib::PropertyMap props = file.file()->properties();
    // 专辑名
    album.albumName = QString::fromStdString(tag->album().toCString(true));
    // 专辑演出者
    album.albumArtists = splitString(QString::fromStdString(props["ALBUMARTIST"].toString().toCString(true)));
    // 流派
    album.genres = splitString(QString::fromStdString(tag->genre().toCString(true)));

    // 发行方
    album.publishers = splitString(QString::fromStdString(props["PUBLISHER"].toString().toCString(true)));

    // 专辑版本
    album.albumVersion = QString::fromStdString(props["ALBUMVERSION"].toString().toCString(true));

    // 专辑封面

    // 发行时间
    unsigned int year = tag->year();
    if (year > 0) {
        DateInfo albumreleasedate;
        albumreleasedate.date = QDate(year, 1, 1);
        albumreleasedate.level = 1;  
        album.albumReleaseTime = albumreleasedate;
    } 
    if (!props["RDATE"].isEmpty()) {
        // 从RDATE中读取时间
        QString dateString = QString::fromStdString(props["RDATE"].toString().toCString(true));
        album.albumReleaseTime = formatDate(dateString);  
    }

    // 国家
    album.country = QString::fromStdString(props["COUNTRY"].toString().toCString(true));

    // 总碟数
    if (!props["TOTALDISCS"].isEmpty()) {
        QString discStr = QString::fromStdString(
            props["TOTALDISCS"].front().toCString(true)
        );
        album.totalDiscs = discStr.toInt();
    } else {
        album.totalDiscs = 0;  // 默认值
    }

    // 语言
    album.languages = splitString(QString::fromStdString(props["LANGUAGE"].toString().toCString(true)));

    // 现场版
    album.liveEvent = QString::fromStdString(props["LIVEEVENT"].toString().toCString(true));
    album.liveEventCount = QString::fromStdString(props["LIVEEVENTCOUNT"].toString().toCString(true));
    
    // 尝试获取albumID
    if (getID) {
        int albumID = ensureAlbumExists(album);
        if (albumID > 0) {
            album.albumID = albumID;
        }
    }

    return album;
}


DateInfo MusicLibrary::formatDate(const QString &dateString){

    QString debugTitle = "formatDate";

    DateInfo date;
    if (dateString.isEmpty()) {
        return date;
    }

    // 使用正则表达式匹配日期格式
    QRegularExpression regex(R"((\d{4})[./-]?(\d{1,2})?[./-]?(\d{1,2})?)");
    QRegularExpressionMatch match = regex.match(dateString);

    if (!match.hasMatch()) {
        return date; // 如果无法匹配，返回无效的 QDate
    }
    
    // 提取年、月、日
    int level = 1;
    int year = match.captured(1).toInt(); 
    int month = match.captured(2).isEmpty() ? 1 : match.captured(2).toInt(); 
    int day = match.captured(3).isEmpty() ? 1 : match.captured(3).toInt();


    if (match.captured(2).isEmpty()) {
        level = 1;
    } else if (match.captured(3).isEmpty()) {
        level = 2;
    } else {
        level = 3;
    }

    // 检查日期的有效性
    if (year < 1 || month < 1 || month > 12 || day < 1 || day > 31) {
        return date; // 返回无效的 QDate
    }

    date.date = QDate(year, month, day);
    date.level = level;

    return date;
}

SongInfo MusicLibrary::readSongInfo(const QString &filePath){

    QString debugTitle = "readSongInfo";
    SongInfo song;

    AlbumInfo album = readAlbumInfo(filePath, true);
    QVector<CharacterInfo> characters = readCharacterInfo(filePath, true);

    TagLib::FileRef file(filePath.toStdString().c_str());

    if(file.isNull() || !file.tag()) {
        qWarning() << debugTitle << " - 无法读取文件标签:" << filePath;
        return song;
    }
    TagLib::Tag* tag = file.tag();
    TagLib::PropertyMap props = file.file()->properties();
    // 标题
    song.title = QString::fromStdString(tag->title().toCString(true));
    // 专辑ID
    song.albumID = album.albumID;
    // 文件信息
    // 时长
    song.durations = file.audioProperties()->lengthInSeconds();
    // 文件大小
    song.fileSize = file.file()->length();
    // 文件路径
    song.filePath = filePath;
    
    // 歌手
    // 后期改为从characters中获取
    song.artists = splitString(QString::fromStdString(tag->artist().toCString(true)));

    // 碟片信息
    // 碟片号
    song.discNumber = props.value("DISCNUMBER").toString().toInt();
    song.totalDiscs = props.value("TOTALDISCS").toString().toInt();
    
    // 曲目号
    song.trackNumber = props.value("TRACKNUMBER").toString().toInt();
    song.totalTracks = props.value("TOTALTRACKS").toString().toInt();
    

    // 是否喜爱
    // FAVORITE、LOVE RATING、LOVE任意一个标签中有值则为喜爱
    song.isFavorite = !props["FAVORITE"].isEmpty() || !props["LOVERATING"].isEmpty() || !props["LOVE"].isEmpty();

    // 是否现场
    // LIVE标签有值则为现场
    song.isLive = !props["LIVE"].isEmpty();

    // 是否单曲
    // SINGLE标签有值则为单曲
    song.isSingle = !props["SINGLE"].isEmpty();
    

    // 是否串烧
    // MASHUP标签有值则为串烧
    song.isMashup = !props["MASHUP"].isEmpty();

    // 是否翻唱
    // COVER标签有值则为翻唱
    song.isCover = !props["COVER"].isEmpty();
    
    
    // Date
    // 发行时间
    unsigned int year = tag->year();
    if (year > 0) {
        DateInfo releaseDate;
        releaseDate.date = QDate(year, 1, 1);
        releaseDate.level = 1;  
        song.releaseDate = releaseDate;
    } 
    if (!props["RDATE"].isEmpty()) {
        // 从RDATE中读取时间
        QString dateString = QString::fromStdString(props["RDATE"].toString().toCString(true));
        song.releaseDate = formatDate(dateString);  
    }
    
    // 原版发行时间
    // 从ORDATE中读取时间
    if (!props["ORDATE"].isEmpty()) {
        QString dateString = QString::fromStdString(props["ORDATE"].toString().toCString(true));
        song.originalReleaseDate = formatDate(dateString);
    }
    

    song.characters = characters;

    // 语言
    song.languages = splitString(QString::fromStdString(props["LANGUAGE"].toString().toCString(true)));

    // 流派
    song.genres = splitString(QString::fromStdString(tag->genre().toCString(true)));

    qDebug() << "readSongInfo --" 
        << "Title:" << song.title
         << ", albumID:" << song.albumID
         << ", duration:" << song.durations
         << ", fileSize:" << song.fileSize
         << ", filePath:" << song.filePath
         << ", discNumber:" << song.discNumber
         << ", totalDiscs:" << song.totalDiscs
         << ", trackNumber:" << song.trackNumber
         << ", totalTracks:" << song.totalTracks
         << ", isFavorite:" << song.isFavorite
         << ", isLive:" << song.isLive
         << ", isSingle:" << song.isSingle
         << ", isMashup:" << song.isMashup
         << ", isCover:" << song.isCover
         << ", releaseDate:" << song.releaseDate.toString()
         << ", originalReleaseDate:" << song.originalReleaseDate.toString();

    return song;
}

QStringList MusicLibrary::splitString(const QString &str){
    QStringList newstr;
    if (!str.isEmpty()) {
        // 使用正则表达式分割字符串（支持 ; , / 等分隔符）
        QRegularExpression regex(splitPartten);
        newstr = str.split(regex, Qt::SkipEmptyParts);
        // 去除前后空格
        for (QString& s : newstr) {
            s = s.trimmed();
        }
    }
    return newstr;
}

QStringList MusicLibrary::splitString(const QStringList &strlist){

    QString debugTitle = "splitString";

    QStringList newstrlist;
    if (!strlist.isEmpty()) {
        foreach(const QString &str, strlist){
            newstrlist.append(splitString(str));
        }
    }
    return newstrlist;
}

QHash<QString, AlbumInfo> MusicLibrary::getAlbumsInfo(const QStringList &musicFiles) {
    QHash<QString, AlbumInfo> albumDatabase;

    foreach(const QString& filePath, musicFiles){
        AlbumInfo album = readAlbumInfo(filePath, false);

        // 生成唯一键（专辑名+首艺术家）
        QString key = album.albumName + "|" + (album.albumArtists.isEmpty() ? "Unknown" : album.albumArtists.first());

        // 合并同一专辑信息
        if(albumDatabase.contains(key)) {
            AlbumInfo& existing = albumDatabase[key];
            // 合并曲目信息
            // track titles: 合并track标题，去重
            for (const QString &trackTitle : album.tracksTitles) {
                if (!existing.tracksTitles.contains(trackTitle)) {
                    existing.tracksTitles.append(trackTitle);
                }
            }
            // track counts: 合并track数目，
            existing.tracksCounts.append(album.tracksCounts);
            // 其他字段合并逻辑...
        } else {
            albumDatabase.insert(key, album);
        }
    }

    // 打印出所有Album的信息
    foreach(const AlbumInfo& album, albumDatabase){
        qDebug() << "getAlbumsInfo -- Name:" << album.albumName
        << ", Artists:" << album.albumArtists
        << ", Version:" << album.albumVersion
        << ", Release Time:" << album.albumReleaseTime.toString()
        << ", Original Release Time:" << album.originalReleaseTime.toString()
        << ", Country:" << album.country
        << ", Total Tracks:" << album.tracksCounts
        << ", Tracks Titles:" << album.tracksTitles
        << ", Genres:" << album.genres
        << ", Languages:" << album.languages
        << ", Live Event:" << album.liveEvent
        << ", Live Event Count:" << album.liveEventCount;
    }

    return albumDatabase;
}



// std::vector<MusicLibrary::SongItem> MusicLibrary::getAllSongs() {

//     QString debugTitle = "getAllSongs";

//     std::vector<SongItem> songs;

//     QSqlQuery query(m_db);
//     query.prepare(
//         "SELECT s.song_id, s.title, s.durations, s.file_path, a.album_name, "
//         "GROUP_CONCAT(DISTINCT g.genre) AS genres, "
//         "GROUP_CONCAT(DISTINCT c.character_name || ':' || sc.role) AS characters "
//         "FROM Songs s "
//         "LEFT JOIN Albums a ON s.album_id = a.album_id "
//         "LEFT JOIN SongCharacters sc ON s.song_id = sc.song_id "
//         "LEFT JOIN Characters c ON sc.character_id = c.character_id "
//         "LEFT JOIN Genre g ON s.song_id = g.song_id "
//         "GROUP BY s.song_id"
//     );

//     if (!query.exec()) {
//         throw std::runtime_error(query.lastError().text().toStdString());
//     }

//     while (query.next()) {
//         SongItem song;
//         song.title = query.value("title").toString();
//         song.duration = query.value("durations").toInt(); 
//         song.filePath = query.value("file_path").toString();

//         QStringList artistRolePairs = query.value("characters").toString().split(",");
//         QMap<QString, QStringList> roleMap;
//         for (const QString &pair : artistRolePairs) {
//             QStringList parts = pair.split(":");
//             if (parts.size() == 2) {
//                 QString name = parts[0].trimmed();
//                 QString role = parts[1].trimmed();
//                 roleMap[name].append(role);
//             }
//         }

//         for (auto it = roleMap.begin(); it != roleMap.end(); ++it) {
//             song.artists.append(it.key() + " (" + it.value().join(", ") + ")");
//         }

//         songs.push_back(song);
//     }

//     return songs;
// }

bool MusicLibrary::insertPlayList(const QString &name, const QVector<int> &songIDs){

    QString debugTitle = "insertPlayList";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return false;
    }

    QSqlQuery query(m_db);

    // 检查歌单是否存在
    query.prepare("SELECT playlist_id FROM PlayList WHERE "
        "name = :playListName");
    query.bindValue(":playListName", name);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check playlist existence failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    int playListID;

    if (query.next()) {
        // 从SongsPlayList中删除旧歌单的歌曲
        playListID = query.value(0).toInt();
        if (songIDs.isEmpty()){
            qDebug() << debugTitle << " - Found Playlist exist, cancel insert: " << playListID;
            return playListID;
        }
        query.prepare("DELETE FROM SongsPlayList WHERE playlist_id = :playListID");
        query.bindValue(":playListID", playListID);
        if (!query.exec()) {
            qDebug() << debugTitle << " - Delete failed:" << query.lastError().text();
        } else {
            qDebug() << debugTitle << " - Delete success" << query.numRowsAffected() << "rows";
        }

    }  else {
        // 在PlayList中添加新歌单
        query.prepare("INSERT INTO PlayList ("
            "name"
            ") VALUES ("
            ":playListName"
            ")"
        );
        query.bindValue(":playListName", name);
        qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
        qDebug() << debugTitle << " - Bound values:" << query.boundValues();
        if (!query.exec()) {
            qWarning() << "Insert playlist failed:" << query.lastError();
            m_db.rollback();
            return -1;
        }
        playListID = query.lastInsertId().toInt();
    }

    if (songIDs.isEmpty()){
        qDebug() << debugTitle << " - Empty Playlist Inserted: " << playListID;
        return playListID;
    }
    

    // 向歌单中添加新歌曲
    for (const int &songID : songIDs){
        query.prepare("INSERT INTO SongsPlayList ("
            "playlist_id, song_id "
            ") VALUES ("
            ":playListID, :songID "
            ")");
        query.bindValue(":playListID", playListID);
        query.bindValue(":songID", songID);
    
        qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
        qDebug() << debugTitle << " - Bound values:" << query.boundValues();
    
        if (!query.exec()) {
            if (query.lastError().nativeErrorCode() == "2067") { // SQLite 的唯一性错误代码
                qDebug() << debugTitle << " - Song already exists, skipping insertion.";
                m_db.rollback();
                return -1;
            } else {
                qWarning() << debugTitle << " - Insert song failed:" << query.lastError();
                m_db.rollback();
                return -1;
            }
        }
    }


    int newSongID = query.lastInsertId().toInt();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Failed to insert song-character:" << query.lastError();
        m_db.rollback();
        return -1;
    }

    if (!m_db.commit()) {
        qWarning() << debugTitle << " - Failed to commit transaction:" << m_db.lastError();
        return -1;
    }

}

bool MusicLibrary::deletePlayList(const int &playListID){

    QString debugTitle = "deletePlayList";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return false;
    }

    QSqlQuery query(m_db);

    // 检查歌单是否存在
    query.prepare("DELETE  FROM Songs WHERE "
        "song_id = :playListID");
    query.bindValue(":playListID", playListID);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Delete playlist failed:" << query.lastError();
        m_db.rollback();
        return -1;
    }
}

QVector<int> MusicLibrary::loadPlayList(const QString &name){

    QString debugTitle = "loadPlayList";

    QVector<int> songIDs;

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return QVector<int>();
    }

    QSqlQuery query(m_db);

    // 检查歌单是否存在
    query.prepare("SELECT playlist_id FROM PlayList WHERE "
        "name = :playListName");
    query.bindValue(":playListName", name);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check playlist existence failed:" << query.lastError();
        m_db.rollback();
        return QVector<int>();
    }
    if (!query.next()) {
        // 不存在
        qWarning() << debugTitle << " - Playlist not exists:";
    }

    // 读取数据
    int playListID = query.value(0).toInt();
    query.prepare("SELECT  song_id FROM SongsPlayList WHERE playlist_id = :playListID");
    query.bindValue(":playListID", playListID);
    if (!query.exec()) {
        qCritical() << debugTitle << " - Failed to execute query:" << query.lastError().text();
    } else {
        while(query.next()){
            const int songID = query.value("song_id").toInt();
            songIDs.append(songID);
        }
    }

    return songIDs;
}

QVector<int> MusicLibrary::loadPlayList(const int &playListID){

    QString debugTitle = "loadPlayList";

    QVector<int> songIDs;

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return QVector<int>();
    }

    QSqlQuery query(m_db);

    // 检查歌单是否存在
    query.prepare("SELECT playlist_id FROM PlayList WHERE "
        "playlist_id = :playlistID");
    query.bindValue(":playlistID", playListID);

    qDebug() << debugTitle << " - Executing query:" << query.lastQuery();
    qDebug() << debugTitle << " - Bound values:" << query.boundValues();

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check playlist existence failed:" << query.lastError();
        m_db.rollback();
        return QVector<int>();
    }
    if (!query.next()) {
        // 不存在
        qWarning() << debugTitle << " - Playlist " << playListID << "not exists:";
    }

    // 读取数据
    query.prepare("SELECT  song_id FROM SongsPlayList WHERE playlist_id = :playListID");
    query.bindValue(":playListID", playListID);
    if (!query.exec()) {
        qCritical() << debugTitle << " - Failed to execute query:" << query.lastError().text();
    } else {
        while(query.next()){
            const int songID = query.value("song_id").toInt();
            songIDs.append(songID);
        }
    }

    return songIDs;
}

QVector<int> MusicLibrary::loadAllSongs(){

    QString debugTitle = "loadAllSongs";

    QVector<int> songIDs;

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return QVector<int>();
    }

    QSqlQuery query(m_db);

    // 读取数据
    query.prepare("SELECT  song_id FROM Songs");

    if (!query.exec()) {
        qCritical() << debugTitle << " - Failed to execute query:" << query.lastError().text();
    } else {
        while(query.next()){
            const int songID = query.value("song_id").toInt();
            songIDs.append(songID);
        }
    }

    return songIDs;
}

QVector<PlayListInfo> MusicLibrary::getPlayLists(){

    QVector<PlayListInfo> playListsInfo;
    QString debugTitle = "getPlayLists";

    if (!m_db.isOpen()) {
        qCritical() << debugTitle << " - Database is not open";
        return QVector<PlayListInfo>();
    }

    QSqlQuery query(m_db);
    query.prepare("SELECT playlist_id, name FROM PlayList ");

    if (!query.exec()) {
        qWarning() << debugTitle << " - Check playlist existence failed:" << query.lastError();
        m_db.rollback();
        return QVector<PlayListInfo>();
    } else {
        while(query.next()){
            playListsInfo.append(PlayListInfo(query.value("playlist_id").toInt(), query.value("name").toString()));
        }
    }

    return playListsInfo;

}