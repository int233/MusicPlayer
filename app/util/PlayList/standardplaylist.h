#ifndef STANDARDPLAYLIST_H
#define STANDARDPLAYLIST_H

#include "QObject"
#include "playlist.h"

class StandardPlayList : public PlayList
{
    Q_OBJECT
public:
    explicit StandardPlayList(const QString& name, QObject* parent = nullptr);
};

#endif