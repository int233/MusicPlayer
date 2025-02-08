#include "musiclibrary.h"

MusicLibrary& MusicLibrary::instance() {
    static MusicLibrary instance;
    return instance;
}

MusicLibrary::MusicLibrary(QObject *parent)
    : QObject{parent},configManager(ConfigManager::instance())
{

}


QStringList MusicLibrary::getLibraryPaths() {
    QString libraryFolderPath = configManager.getSettings()->value("Common/LibraryPath/").toString();

    const QString librarySubfix = "intlibrary";
    QStringList filePaths;

    // 检查目录是否存在
    QDir libraryDir(libraryFolderPath);
    if (!libraryDir.exists()) {
        qWarning() << "Library directory does not exist:" << libraryFolderPath;
        return filePaths; // 返回空列表
    }

    // 使用 QDirIterator 遍历目录
    QDirIterator it(libraryFolderPath, QStringList() << "*." + librarySubfix, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        filePaths.append(it.next());
    }

    return filePaths;

}
