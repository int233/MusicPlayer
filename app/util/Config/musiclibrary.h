#ifndef MUSICLIBRARY_H
#define MUSICLIBRARY_H

#include <QObject>

#include "app/util/Config/configmanager.h"

class MusicLibrary : public QObject
{
    Q_OBJECT

public:
    static MusicLibrary& instance();

private:
    explicit MusicLibrary(QObject *parent = nullptr);
    ConfigManager &configManager;

    QStringList getLibraryPaths();

signals:
};

#endif // MUSICLIBRARY_H
