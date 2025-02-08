#include "musicscanner.h"

MusicScanner::MusicScanner(QObject *parent)
    : QObject{parent}
{

}

void MusicScanner::startScan(const QString &directory) {
    QDir dir(directory);
    if (!dir.exists()) {
        qWarning() << "Directory does not exist:" << directory;
        emit finished();
        return;
    }

    // 递归扫描目录
    scanDirectory(directory);
    emit finished();
}

void MusicScanner::scanDirectory(const QString &path) {
    QDir dir(path);
    QFileInfoList entries = dir.entryInfoList(
        QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot
        );

    for (const QFileInfo &entry : entries) {
        if (entry.isDir()) {
            scanDirectory(entry.absoluteFilePath());
        } else if (entry.isFile() && m_supportedFormats.contains(entry.suffix())) {
            emit fileFound(entry.absoluteFilePath());
        }
    }
}
