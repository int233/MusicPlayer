#include "playlist.h"

PlayList::PlayList(const QString& name, QObject* parent)
    : QObject{parent}, m_name(name), musicLib(MusicLibrary::instance())
{
    m_id = getPlayListID();
    m_lastModified = QDateTime::currentSecsSinceEpoch();
}

int PlayList::id() const { return m_id; }
QString PlayList::name() const { return m_name; }

void PlayList::setName(const QString& name) { 
    if(m_name != name) {
        m_name = name;
        emit PlayListChanged();
    }
}

int PlayList::getPlayListID(){
    return 1;
}

QHash<int, MusicLibrary::SongInfo>  PlayList::getSongs() const { return m_songs; }

QVector<int>  PlayList::getSongIDs() const { return m_songIDs; }

int PlayList::addSong(const QString& songPath) {
    MusicLibrary::SongInfo songInfo = musicLib.getSingleSong(songPath);
    
    if (m_songs.contains(songInfo.songID)){
        qWarning() << "Duplicated song ID: " << songInfo.songID;
        emit InsertDuplicatedSong();
        return 0;
    }

    m_songs.insert(songInfo.songID, songInfo);
    m_songIDs.append(songInfo.songID);

    emit InsertSong();
    emit PlayListChanged();
}

int PlayList::addSong(const int& songID) {
    MusicLibrary::SongInfo songInfo = musicLib.getSingleSong(songID);
    
    if (m_songs.contains(songInfo.songID)){
        qWarning() << "Duplicated song ID: " << songInfo.songID;
        emit InsertDuplicatedSong();
        return 0;
    }

    m_songs.insert(songInfo.songID, songInfo);
    m_songIDs.append(songInfo.songID);
    emit InsertSong();
    emit PlayListChanged();
}

int PlayList::removeSongs(const QVector<int> &songIDs){

    // get valid songIDs
    QVector<int> validSongIDs;
    for (const int &songID : songIDs){
        if (m_songs.contains(songID)){
            validSongIDs.append(songID);
        }
    }

    // remove songs
    
    for (const int &songID : validSongIDs){
        m_songIDs.removeAll(songID);
        m_songs.remove(songID);
    }
    emit PlayListChanged();
    emit RemovedSong(songIDs);
    return 1;
}

int PlayList::moveSongs(const QVector<int> songIDs, const int songIDBeforeDes){

    // get index of old songIDs
    // QVector<int> songIDIndex;
    // for (int &songID : songIDs){
    //     songIDIndex.append(songIDs.indexOf(songID))
    // }
    // auto minIt = std::min_element(songIDIndex.begin(), songIDIndex.end());
    // auto maxIt = std::max_element(songIDIndex.begin(), songIDIndex.end());

    // delete old songIDs
    for (const int &songID : songIDs){
        m_songIDs.removeAll(songID);
    }

    // Destination index
    int desIndex = songIDs.indexOf(songIDBeforeDes);
    for (auto it = songIDs.rbegin(); it != songIDs.rend(); ++it) {
        m_songIDs.insert(desIndex, *it); 
    }
    emit PlayListChanged();
}

int PlayList::saveToLibrary(){
    musicLib.insertPlayList(m_name, m_songIDs);
}

int PlayList::loadFromLibrary(){
    m_songIDs.clear();
    m_songs.clear();
    QVector<int> songIDs = musicLib.loadPlayList(m_name);
    for (const int &songID : songIDs){
        addSong(songID);
    }
    emit PlayListChanged();
}

