#include "musicscanner.h"
MusicScanner &MusicScanner::instance() {
    static MusicScanner instance;
    return instance;
}

MusicScanner::MusicScanner(QObject *parent)
    : QObject{parent},configManager(ConfigManager::instance())
{
    intSetting = configManager.getSettings();
}


MusicScanner::~MusicScanner() {}


void MusicScanner::startScan()
{
    emit scanMusicStarted();

    QStringList musicFolders = intSetting->value("Common/MusicFolderPath").toStringList();

    QStringList musicFiles;

    // 扫描文件阶段
    for (const QString &folderPath : musicFolders) {
        QDir dir(folderPath);
        if (!dir.exists()) {
            emit scanMusicFailed(QString("Directory not exists: %1").arg(folderPath));
            continue;
        }

        QDirIterator it(folderPath, m_supportedFormats, QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            if (maxFiles > 0 && musicFiles.size() >= maxFiles) break;
            musicFiles.append(it.next());
        }

        if (maxFiles > 0 && musicFiles.size() >= maxFiles) break;
    }

    emit scanMusicFinished(musicFiles.size());

    if (musicFiles.isEmpty()) {
        emit scanTagFailed("No music files found");
        return;
    }

    emit scanTagStarted();

    int tagsRead = 0;
    for (const QString &filePath : musicFiles) {
        TagLib::FileRef fileRef(filePath.toStdString().c_str());
        if (fileRef.isNull() || !fileRef.tag()) {
            emit scanTagFailed(QString("Failed to read tags: %1").arg(filePath));
            continue;
        }

        TagLib::Tag *tag = fileRef.tag();
        QString title = QString::fromUtf8(tag->title().toCString(true));
        QString artist = QString::fromUtf8(tag->artist().toCString(true));
        QString album = QString::fromUtf8(tag->album().toCString(true));
        qDebug() << "FilePath: " << filePath << ", Title: " << title << ", Artist: " << artist << ", Album: " << album;
        emit musicFileTagRead(filePath, title, artist, album);
        tagsRead++;
    }

    if (tagsRead > 0) {
        emit scanTagFinished(tagsRead);
    } else {
        emit scanTagFailed("No valid tags found in any files");
    }
}
