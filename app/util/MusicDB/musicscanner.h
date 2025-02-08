#ifndef SCANMUSIC_H
#define SCANMUSIC_H

#include <QObject>
#include <QDir>
#include <QFileInfo>
#include <QStringList>

class MusicScanner : public QObject
{
    Q_OBJECT
public:
    explicit MusicScanner(QObject *parent = nullptr);

signals:
    void progressUpdated(int percent);
    void fileFound(const QString &filePath);
    void finished();

public slots:
    void startScan(const QString &directory);

private:
    void scanDirectory(const QString &path);
    QStringList m_supportedFormats = {"*.mp3", "*.flac", "*.wav"};
};

#endif // SCANMUSIC_H
