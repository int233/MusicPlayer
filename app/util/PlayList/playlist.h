#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>
#include <QHash>
#include <QVector>

#include <algorithm>

#include "app/util/Config/musicdatastruct.h"

class MusicLibrary;
struct SongInfo;


class PlayList : public QObject
{
    Q_OBJECT
public:
    explicit PlayList(const QString& name, QObject* parent = nullptr);
    
    int id() const;
    QString name() const;
    void setName(const QString& name);

    QHash<int, SongInfo> getSongs() const;
    QVector<int> getSongIDs() const;
    
    int addSong(const QString& songPath);
    int addSong(const int& songID);

    int moveSongs(const QVector<int> songIDs, const int newPos);
    
    int removeSongs(const QVector<int> &songIDs);

    virtual bool isSmart() const { return false; }

private:

signals:
    void PlayListChanged();
    void InsertDuplicatedSong();
    void InsertSong();
    void RemovedSong(const QVector<int> &songIDs);

protected:
    MusicLibrary &musicLib;
    // MusicLibrary::PlayList playListInfo;
    int m_id;
    QString m_name;
    // 顺序标记
    QVector<int> m_songIDs;
    // 歌曲信息
    QHash<int, SongInfo> m_songs;

    qint64 m_lastModified;

    int getPlayListID();
    int saveToLibrary();
    void loadFromLibrary();
};

#endif