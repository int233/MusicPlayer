#ifndef MINIPLAYER_H
#define MINIPLAYER_H

#include <QWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QSlider>
#include <QPushButton>
#include <QTimeEdit>

#include <QHBoxLayout>

#include "app/util/MusicPlayer/musicplayer.h"

class MiniPlayer : public QWidget
{
    Q_OBJECT
public:
    explicit MiniPlayer(QWidget *parent = nullptr);
    MusicPlayer *player;
    QSlider* getProgressSlider() const { return MiniPlayerProgress; }
    QTimeEdit* getCurrentTimeEdit() const { return MiniPlayerCurrentTime; }
    QTimeEdit* getTotalTimeEdit() const { return MiniPlayerTotalTime; }
    QPushButton* getPauseButton() const { return MiniPlayerPause; }

private slots:
    void togglePlayPause(bool play);  // 播放/暂停按钮的槽函数

private:
    // MiniPlayer 控件
    QWidget *MiniAlbum;
    QGraphicsView *AlbumImage;
    QWidget *MusicTitles;
    QLabel *PlayingMusicTitle;
    QLabel *PlayingAblumTitle;
    QWidget *MiniSlider;
    QTimeEdit *MiniPlayerCurrentTime;
    QSlider *MiniPlayerProgress;
    QTimeEdit *MiniPlayerTotalTime;
    QWidget *MiniControler;
    QPushButton *MiniPlayerOrderMode;
    QPushButton *MiniPlayerPrevious;
    QPushButton *MiniPlayerPause;
    QPushButton *MiniPlayerNext;
    QPushButton *MiniPlayerSound;
};

#endif // MINIPLAYER_H


