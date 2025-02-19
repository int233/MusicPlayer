#include "musicservice.h"

MusicService& MusicService::instance() {
    static MusicService instance;
    return instance;
}

MusicService::MusicService(QObject *parent)
    : QMediaPlayer{parent},configManager(ConfigManager::instance()),audioOutput(new QAudioOutput(this))
{
    setAudioOutput(audioOutput);
}

MusicService::~MusicService()
{
    delete audioOutput;
}

void MusicService::setAudioSource(const QString &musicPath){
    QMediaPlayer::setSource(QURL::fromLocalFile(musicPath));
}
void MusicService::setAudioSource(const QUrl &musicPath){
    QMediaPlayer::setSource(musicPath);
}

void MusicService::play()
{
    QMediaPlayer::play();
}

void MusicService::pause()
{
    QMediaPlayer::pause();
}

void MusicService::stop()
{
    QMediaPlayer::stop();
}

void MuiscService::onMediaStatusChanged(QMediaPlayer::MediaStatus status)
{
    if (status == QMediaPlayer::LoadedMedia) {
        qint64 durationMs = this->duration();
        qDebug() << "Media loaded. Duration (ms):" << durationMs;

        if (miniPlayerProgress) {
            miniPlayerProgress->setMaximum(static_cast<int>(durationMs / 1000));
            MuiscService::initProgressBar(durationMs, 0);
        }
    } else if (status == QMediaPlayer::InvalidMedia) {
        qDebug() << "Failed to load media.";
    }
}

