#ifndef MUSICSERVICE_H
#define MUSICSERVICE_H

#include <QObject>

#include <QMediaPlayer>
#include <QAudioOutput>
#include <QSlider>
#include <QTimeEdit>
#include <QDebug>

#include "app/util/Config/configmanager.h"

class MuiscService : public QMediaPlayer
{
    Q_OBJECT
public:
    static MuiscService& instance();
    ~MusicService();

    // 设置音乐文件的 URL
    void setAudioSource(const QString &musicPath);
    void setAudioSource(const QUrl &musicPath);

    // 播放音乐
    void play(); 
    // 暂停播放
    void pause();
    // 停止播放
    void stop(); 
    // 设置音量
    void setVolume(int volume);      

private:
    explicit MusicService(QObject *parent = nullptr);
    ConfigManager &configManager;
    QAudioOutput *audioOutput;


signals:


private slots:
    void onMediaStatusChanged(QMediaPlayer::MediaStatus status);

};

#endif // MUSICSERVICE_H
