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
        "    publisher      TEXT,"
        "    album_score    REAL CHECK(album_score BETWEEN 0 AND 5),"
        "    album_version  TEXT,"
        "    original_release_time   DATETIME,"
        "    album_release_time   DATETIME,"
        "    country        TEXT,"
        "    genres          TEXT,"
        "    total_tracks   INTEGER,"
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
    */
    query.exec(
        "CREATE TABLE IF NOT EXISTS Characters ("
        "    character_id             INTEGER PRIMARY KEY AUTOINCREMENT,"
        "    character_name           TEXT CHECK(character_name != ''),"
        "    character_foreign_name   TEXT CHECK(character_foreign_name != ''),"
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
        "    duration_ms    INTEGER CHECK(duration_ms > 0),"
        "    file_size      INTEGER CHECK(file_size > 0),"
        "    file_path      TEXT NOT NULL UNIQUE CHECK(file_path != ''),"
        "    album_id       INTEGER REFERENCES Albums(album_id) ON DELETE SET NULL,"
        "    track_number   INTEGER CHECK(track_number > 0),"
        "    disc_number    INTEGER CHECK(disc_number > 0),"
        "    is_favorite    BOOLEAN DEFAULT 0,"
        "    is_live        BOOLEAN DEFAULT 0,"
        "    is_single      BOOLEAN DEFAULT 0,"
        "    language       TEXT,"
        "    live_event     TEXT,"
        "    live_event_count TEXT,"
        "    is_cover       BOOLEAN DEFAULT 0,"
        "    is_mashup      BOOLEAN DEFAULT 0,"
        "    release_time   DATETIME,"
        "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    modified_at     DATETIME DEFAULT CURRENT_TIMESTAMP"
        ");"
        );

    // 多对多关系表
    // SongCharacters
    // 歌曲与角色对应
    query.exec(
        "CREATE TABLE IF NOT EXISTS SongCharacters ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    character_id   INTEGER REFERENCES Characters(character_id) ON DELETE CASCADE,"
        "    role           TEXT CHECK(role IN ('performer', 'composer', 'lyricist', 'original_artist')),"
        "    PRIMARY KEY (song_id, character_id, role)"
        ");"
        );

    // SongMetadata
    // Genre
    query.exec(
        "CREATE TABLE IF NOT EXISTS Genre ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    genre          TEXT,"
        "    PRIMARY KEY (song_id, genre)"
        ");"
        );

    // Language
    query.exec(
        "CREATE TABLE IF NOT EXISTS Language ("
        "    song_id        INTEGER REFERENCES Songs(song_id) ON DELETE CASCADE,"
        "    language          TEXT,"
        "    PRIMARY KEY (song_id, language)"
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
    if (songInfo.albumId > 0) {
        QSqlQuery checkQuery(m_db);
        checkQuery.prepare("SELECT 1 FROM Albums WHERE album_id = ?");
        checkQuery.addBindValue(songInfo.albumId);
        if (checkQuery.exec() && checkQuery.next()) {
            return songInfo.albumId;
        }
    }

    // 需要创建新专辑的检查
    if (songInfo.albumId == -1) {
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

int MusicLibrary::ensureCharacterExists(const QString &characterName, const QString &foreignName)
{
    // 检查输入有效性
    if (characterName.isEmpty() && foreignName.isEmpty()) {
        throw std::runtime_error("Character name and foreign name cannot both be empty");
    }

    // 检查角色是否存在（通过中文名或外文名）
    QSqlQuery checkQuery(m_db);
    checkQuery.prepare(
        "SELECT character_id FROM Characters "
        "WHERE character_name = ? OR character_foreign_name = ?"
        );
    checkQuery.addBindValue(characterName.isEmpty() ? foreignName : characterName);
    checkQuery.addBindValue(foreignName.isEmpty() ? characterName : foreignName);

    if (checkQuery.exec()) {
        while (checkQuery.next()) {
            int existingId = checkQuery.value(0).toInt();
            // 更新外文名或中文名如果存在空值
            QSqlQuery updateQuery(m_db);
            if (characterName.isEmpty()) {
                updateQuery.prepare(
                    "UPDATE Characters SET character_name = ? "
                    "WHERE character_id = ?"
                    );
                updateQuery.addBindValue(characterName);
            } else if (foreignName.isEmpty()) {
                updateQuery.prepare(
                    "UPDATE Characters SET character_foreign_name = ? "
                    "WHERE character_id = ?"
                    );
                updateQuery.addBindValue(foreignName);
            }
            if (!updateQuery.exec()) {
                qWarning() << "Update character failed:" << updateQuery.lastError();
            }
            return existingId;
        }
    }

    // 插入新角色
    QSqlQuery insertQuery(m_db);
    insertQuery.prepare(
        "INSERT INTO Characters (character_name, character_foreign_name) "
        "VALUES (:name, :foreignName)"
        );
    insertQuery.bindValue(":name", characterName.isEmpty() ? QVariant() : characterName);
    insertQuery.bindValue(":foreignName", foreignName.isEmpty() ? QVariant() : foreignName);

    if (!insertQuery.exec()) {
        throw std::runtime_error("Insert character failed: " + insertQuery.lastError().text().toStdString());
    }

    return insertQuery.lastInsertId().toInt();
}

bool MusicLibrary::insertSong(const SongInfo &songInfo)
{
    QSqlDatabase::database(m_db.connectionName()).transaction();

    try {
        // 检查文件路径唯一性
        QSqlQuery checkQuery(m_db);
        checkQuery.prepare("SELECT COUNT(*) FROM Songs WHERE file_path = ?");
        checkQuery.addBindValue(songInfo.filePath);
        if (!checkQuery.exec() || !checkQuery.next()) {
            throw std::runtime_error("File path check failed: " + checkQuery.lastError().text().toStdString());
        }
        if (checkQuery.value(0).toInt() > 0) {
            throw std::runtime_error("Duplicate file path: " + songInfo.filePath.toStdString());
        }

        // 检查Album是否已经存在，若存在，则获取对应id，若不存在，则插入Album
        // 处理专辑信息
        int albumId = -1;
        try {
            albumId = ensureAlbumExists(songInfo);
        } catch (const std::exception &e) {
            throw std::runtime_error(std::string("Album validation failed: ") + e.what());
        }

        // 插入主歌曲信息
        QSqlQuery songQuery(m_db);
        songQuery.prepare(
            "INSERT INTO Songs ("
            "  title, duration_ms, file_size, file_path, album_id, track_number, disc_number,"
            "  is_favorite, is_live, is_single, live_event, live_event_count, is_cover,"
            "  is_mashup, release_time"
            ") VALUES ("
            "  :title, :durationMs, :fileSize, :filePath, :albumId, :trackNumber, :discNumber,"
            "  :isFavorite, :isLive, :isSingle, :liveEvent, :liveEventCount, :isCover,"
            "  :isMashup, :releaseTime"
            ")"
            );

        songQuery.bindValue(":title", songInfo.title);
        songQuery.bindValue(":durationMs", songInfo.durationMs);
        songQuery.bindValue(":fileSize", songInfo.fileSize);
        songQuery.bindValue(":filePath", songInfo.filePath);
        songQuery.bindValue(":albumId", songInfo.albumId > 0 ? songInfo.albumId : QVariant());
        songQuery.bindValue(":trackNumber", songInfo.trackNumber);
        songQuery.bindValue(":discNumber", songInfo.discNumber);
        songQuery.bindValue(":isFavorite", songInfo.isFavorite);
        songQuery.bindValue(":isLive", songInfo.isLive);
        songQuery.bindValue(":isSingle", songInfo.isSingle);
        songQuery.bindValue(":liveEvent", songInfo.liveEvent);
        songQuery.bindValue(":liveEventCount", songInfo.liveEventCount);
        songQuery.bindValue(":isCover", songInfo.isCover);
        songQuery.bindValue(":isMashup", songInfo.isMashup);
        songQuery.bindValue(":releaseTime", songInfo.releaseTime);

        if (!songQuery.exec()) {
            throw std::runtime_error("Insert song failed: " + songQuery.lastError().text().toStdString());
        }

        const int songId = songQuery.lastInsertId().toInt();

        qDebug() << "Song ID: " << songId;

        // 插入角色关联
        for (auto it = songInfo.characters.constBegin(); it != songInfo.characters.constEnd(); ++it) {
            const QString &role = it.key();
            const QString &characterName = it.value().first;
            const QString &foreignName = it.value().second;

            try {
                int charId = ensureCharacterExists(characterName, foreignName);

                QSqlQuery scQuery(m_db);
                scQuery.prepare(
                    "INSERT INTO SongCharacters (song_id, character_id, role) "
                    "VALUES (?, ?, ?)"
                    );
                scQuery.addBindValue(songId);
                scQuery.addBindValue(charId);
                scQuery.addBindValue(role);

                if (!scQuery.exec()) {
                    throw std::runtime_error("Insert song-character failed: " + scQuery.lastError().text().toStdString());
                }
            } catch (const std::exception &e) {
                qWarning() << "Failed to process character:" << e.what();
            }
        }

        // 插入流派
        foreach (const QString &genre, songInfo.genres) {
            QSqlQuery genreQuery(m_db);
            genreQuery.prepare(
                "INSERT OR IGNORE INTO Genre (song_id, genre) "
                "VALUES (?, ?)"
                );
            genreQuery.addBindValue(songId);
            genreQuery.addBindValue(genre);
            if (!genreQuery.exec()) {
                throw std::runtime_error("Insert genre failed: " + genreQuery.lastError().text().toStdString());
            }
        }

        // 插入语言
        foreach (const QString &lang, songInfo.languages) {
            QSqlQuery langQuery(m_db);
            langQuery.prepare(
                "INSERT OR IGNORE INTO Language (song_id, language) "  // 注意表名需要修正
                "VALUES (?, ?)"
                );
            langQuery.addBindValue(songId);
            langQuery.addBindValue(lang);
            if (!langQuery.exec()) {
                throw std::runtime_error("Insert language failed: " + langQuery.lastError().text().toStdString());
            }
        }

        QSqlDatabase::database(m_db.connectionName()).commit();
        return true;
    } catch (const std::exception &e) {
        qCritical() << "Insert song error:" << e.what();
        QSqlDatabase::database(m_db.connectionName()).rollback();
        return false;
    }
}

bool MusicLibrary::insertScanResult(const QStringList &musicFiles){

    if (!m_db.isOpen()) {
        qCritical() << "Database is not open";
        return false;
    }

    QSqlQuery query(m_db);

    // 读取所有的专辑信息
    QHash albumDatabase = getAlbumsInfo(musicFiles);
    
    // 读取所有的人物信息

    // 向数据库中插入专辑和人物

    // 读取所有的歌曲信息

    // 将专辑和人物替换为数据库中的ID

    // 向数据库中插入歌曲信息
    
    return true;
}

MusicLibrary::AlbumInfo MusicLibrary::readAlbumInfo(const QString &filePath){
    AlbumInfo album;

    TagLib::FileRef file(filePath.toStdString().c_str());

    if(file.isNull() || !file.tag()) {
        qWarning() << "无法读取文件标签:" << filePath;
        return album;
    }

    TagLib::Tag* tag = file.tag();
    TagLib::PropertyMap props = file.file()->properties();
    // 专辑名
    album.albumName = QString::fromStdString(tag->album().toCString(true));
    // 专辑演出者
    album.albumArtists << QString::fromStdString(tag->artist().toCString(true));
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
        MusicLibrary::DateInfo albumreleasedate;
        albumreleasedate.date = QDate(year, 1, 1);
        albumreleasedate.level = 1;  
        album.albumReleaseTime = albumreleasedate;
    } 
    if (!props["DATE"].isEmpty()) {
        // 从DATE中读取时间
        QString dateString = QString::fromStdString(props["DATE"].toString().toCString(true));
        album.albumReleaseTime = formatDate(dateString);  
    }
    // album.albumReleaseTime = QDateTime::fromString(QString::fromStdString(tag->year()), "yyyy");

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
    
    return album;
}


MusicLibrary::DateInfo MusicLibrary::formatDate(const QString &dateString){

    MusicLibrary::DateInfo date;
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

MusicLibrary::SongInfo MusicLibrary::readSongInfo(const QString &filePath){
    SongInfo song;

    TagLib::FileRef file(filePath.toStdString().c_str());

    if(file.isNull() || !file.tag()) {
        qWarning() << "无法读取文件标签:" << filePath;
        return song;
    }

    TagLib::Tag* tag = file.tag();
    TagLib::PropertyMap props = file.file()->properties();

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

    QStringList newstrlist;
    if (!strlist.isEmpty()) {
        foreach(const QString &str, strlist){
            newstrlist.append(splitString(str));
        }
    }
    return newstrlist;
}

QHash<QString, MusicLibrary::AlbumInfo> MusicLibrary::getAlbumsInfo(const QStringList &musicFiles) {
    QHash<QString, AlbumInfo> albumDatabase;

    foreach(const QString& filePath, musicFiles){
        AlbumInfo album = readAlbumInfo(filePath);

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
        qDebug() << "Album Name: " << album.albumName;
        qDebug() << "Album Artists: " << album.albumArtists;
        qDebug() << "Album Version: " << album.albumVersion;
        qDebug() << "Album Release Time: " << album.albumReleaseTime.toString();
        qDebug() << "Original Release Time: " << album.originalReleaseTime.toString();
        qDebug() << "Country: " << album.country;
        qDebug() << "Total Tracks: " << album.tracksCounts;
        qDebug() << "Tracks Titles: " << album.tracksTitles;
        qDebug() << "Genres: " << album.genres;
        qDebug() << "Languages: " << album.languages;
        qDebug() << "Live Event: " << album.liveEvent;
        qDebug() << "Live Event Count: " << album.liveEventCount;
    }

    return albumDatabase;
}



std::vector<MusicLibrary::SongItem> MusicLibrary::getAllSongs()
{
    std::vector<SongItem> songs;

    QSqlQuery query(m_db);
    query.prepare(
        "SELECT s.title, s.duration_ms, s.file_path, "
        "GROUP_CONCAT(DISTINCT c.character_name) AS artists "
        "FROM Songs s "
        "LEFT JOIN SongCharacters sc ON s.song_id = sc.song_id "
        "LEFT JOIN Characters c ON sc.character_id = c.character_id "
        "WHERE sc.role = 'performer' "
        "GROUP BY s.song_id"
        );

    if (!query.exec()) {
        throw std::runtime_error(query.lastError().text().toStdString());
    }

    while (query.next()) {
        SongItem song;
        song.title = query.value("title").toString();
        song.durationMs = query.value("duration_ms").toInt();
        song.filePath = query.value("file_path").toString();
        song.artists = query.value("artists").toString().split(",");
        songs.push_back(song);
    }

    return songs;
}
