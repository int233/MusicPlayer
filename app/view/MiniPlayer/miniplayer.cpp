#include "miniplayer.h"

MiniPlayer::MiniPlayer(QWidget *parent)
    : QWidget{parent}, player(new MusicPlayer(this))
{
    // MiniPlayer 布局
    QHBoxLayout *MiniPlayerLayout = new QHBoxLayout(this);

    // MiniAlbum
    MiniAlbum = new QWidget();
    QHBoxLayout *MiniAlbumLayout = new QHBoxLayout(MiniAlbum);
    AlbumImage = new QGraphicsView(MiniAlbum);
    MusicTitles = new QWidget();
    QVBoxLayout *MusicTitlesLayout = new QVBoxLayout(MusicTitles);
    PlayingMusicTitle = new QLabel("TextLabel", MusicTitles);
    PlayingAblumTitle = new QLabel("TextLabel", MusicTitles);
    MusicTitlesLayout->addWidget(PlayingMusicTitle);
    MusicTitlesLayout->addWidget(PlayingAblumTitle);
    MiniAlbumLayout->addWidget(AlbumImage);
    MiniAlbumLayout->addWidget(MusicTitles);
    MiniPlayerLayout->addWidget(MiniAlbum);

    // MiniSlider
    MiniSlider = new QWidget();
    QHBoxLayout *MiniSliderLayout = new QHBoxLayout(MiniSlider);
    MiniPlayerCurrentTime = new QTimeEdit(MiniSlider);
    MiniPlayerCurrentTime->setButtonSymbols(QAbstractSpinBox::NoButtons);
    MiniPlayerProgress = new QSlider(Qt::Horizontal, MiniSlider);
    MiniPlayerTotalTime = new QTimeEdit(MiniSlider);
    MiniPlayerTotalTime->setButtonSymbols(QAbstractSpinBox::NoButtons);
    MiniSliderLayout->addWidget(MiniPlayerCurrentTime);
    MiniSliderLayout->addWidget(MiniPlayerProgress);
    MiniSliderLayout->addWidget(MiniPlayerTotalTime);
    MiniPlayerLayout->addWidget(MiniSlider);

    // MiniControler
    MiniControler = new QWidget();
    QHBoxLayout *MiniControlerLayout = new QHBoxLayout(MiniControler);
    MiniPlayerOrderMode = new QPushButton(MiniControler);
    MiniPlayerPrevious = new QPushButton(MiniControler);
    MiniPlayerPause = new QPushButton(MiniControler);
    MiniPlayerNext = new QPushButton(MiniControler);
    MiniPlayerSound = new QPushButton(MiniControler);
    MiniControlerLayout->addWidget(MiniPlayerOrderMode);
    MiniControlerLayout->addWidget(MiniPlayerPrevious);
    MiniControlerLayout->addWidget(MiniPlayerPause);
    MiniControlerLayout->addWidget(MiniPlayerNext);
    MiniControlerLayout->addWidget(MiniPlayerSound);
    MiniPlayerLayout->addWidget(MiniControler);

    // MusicPlayer player;
    player->setMiniPlayer(MiniPlayerProgress, MiniPlayerCurrentTime, MiniPlayerTotalTime);
    player->setSource(QUrl::fromLocalFile("D:\\Music\\张学友\\学友 经典 世界巡回演唱会 香港站 再见篇 (Live)\\张学友 - Medley (友情歌2014 -1985) (Live).flac"));
    player->setVolume(20);

    // 连接信号槽
    connect(MiniPlayerPause, &QPushButton::clicked, this, &MiniPlayer::togglePlayPause);

}

void MiniPlayer::togglePlayPause(bool play) {
    if (player->playbackState() == QMediaPlayer::PlayingState && !play) {
        player->pause();
    } else if ((player->playbackState() == QMediaPlayer::StoppedState) && play){
        player->play();
    } else if ((player->playbackState() == QMediaPlayer::PausedState) && play) {
        player->play();
    } else {
        // 如果没有明确指定，则根据当前状态切换
        if (player->playbackState() == QMediaPlayer::PlayingState) {
            player->pause();
        } else {
            player->play();
        }
    }
}
