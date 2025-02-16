#include "musiclibrary.h"

MusicLibrary& MusicLibrary::instance() {
    static MusicLibrary instance;
    return instance;
}

MusicLibrary::MusicLibrary(QObject *parent)
    : QObject{parent},configManager(ConfigManager::instance())
{
    m_db = QSqlDatabase::addDatabase("QSQLITE", DB_CONNECTION_NAME);
    QString databasePath;
    QStringList libraryPaths = getLibraryPaths();
    if (libraryPaths.isEmpty()) {
        databasePath = configManager.GetMusicFolderLibraryPath()+"test.intlibrary";
    } else {
        databasePath = libraryPaths.first();
    }
    qDebug() << "Library Path: " << databasePath;
    m_db.setDatabaseName(databasePath);

    if (!m_db.open()) {
        qCritical() << "Database connection error:" << m_db.lastError().text();
        return;
    }

    initializeDatabase();
}

MusicLibrary::~MusicLibrary()
{
    m_db.close();
    QSqlDatabase::removeDatabase(DB_CONNECTION_NAME);
}

void MusicLibrary::initializeDatabase()
{
    QSqlQuery query(m_db);
    query.exec("PRAGMA foreign_keys = ON;");
    createTables();
}


QStringList MusicLibrary::getLibraryPaths() {
    QString libraryFolderPath = configManager.getSettings()->value("Common/LibraryPath/").toString();

    const QString librarySubfix = "intlibrary";
    QStringList filePaths;

    // 检查目录是否存在
    QDir libraryDir(libraryFolderPath);
    if (!libraryDir.exists()) {
        qWarning() << "Library directory does not exist:" << libraryFolderPath;
        return filePaths; // 返回空列表
    }

    // 使用 QDirIterator 遍历目录
    QDirIterator it(libraryFolderPath, QStringList() << "*." + librarySubfix, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        filePaths.append(it.next());
    }

    return filePaths;

}

void MusicLibrary::createTables()
{
    QSqlQuery query(m_db);

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
        "    release_time   DATETIME,"
        "    total_tracks   INTEGER CHECK(total_tracks > 0),"
        "    total_discs    INTEGER CHECK(total_discs > 0),"
        "    created_at     DATETIME DEFAULT CURRENT_TIMESTAMP,"
        "    modified_at    DATETIME DEFAULT CURRENT_TIMESTAMP"
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
    if (songInfo.albumName.isEmpty()) {
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

    query.bindValue(":name", songInfo.albumName);
    query.bindValue(":publisher", songInfo.publisher);
    query.bindValue(":version", songInfo.albumVersion);
    query.bindValue(":releaseTime", songInfo.albumReleaseTime);
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
