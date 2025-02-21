#include "musicdatastruct.h"
// #include "app/util/PlayList/playlist.h"
#include "app/util/PlayList/standardplaylist.h"
#include "app/util/PlayList/smartplaylist.h"

PlayListInfo::PlayListInfo(const int &id, const QString &name, bool isSmart, QObject* parent)
    : playListID(id), playListName(name), isSmart(isSmart)
{
    songIDs = QVector<int>();
    if (isSmart) {
        playlist = new SmartPlayList(name, parent);
    } else {
        playlist = new StandardPlayList(name, parent);
    }
}

// PlayListInfo::~PlayListInfo() {
//     delete playlist;
// }