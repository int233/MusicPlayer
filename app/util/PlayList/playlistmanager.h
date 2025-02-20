#ifndef PLAYLISTMANAGER_H
#define PLAYLISTMANAGER_H

#include <QObject>

#include "app/util/Config/configmanager.h"
#include "app/util/Config/musiclibrary.h"

class PlayListManager : public QObject
{
    Q_OBJECT

public:
    static PlayListManager& instance();



private:
    explicit PlayListManager(QObject *parent = nullptr);
    ConfigManager &configManager;
    MusicLibrary &musicLib;


};

#endif