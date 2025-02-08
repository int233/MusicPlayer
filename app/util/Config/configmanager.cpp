#include "configmanager.h"

ConfigManager& ConfigManager::instance() {
    static ConfigManager instance;
    return instance;
}

ConfigManager::ConfigManager(QObject *parent)
    : QObject{parent}
{
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    QString path = GetConfigPath();

    QString logMessage = QString("Time: %1 | ConfigPath: %2")
                             .arg(currentTime).arg(path);

    m_settings = new QSettings(path, QSettings::IniFormat, this);
    m_settings->setValue("Common/Title", "IntMusic");

    SaveConfig(path, m_settings);

    emit settingsChanged();
}

QString ConfigManager::configPath() const {
    return m_settings ? m_settings->fileName() : QString();
}

QSettings* ConfigManager::getSettings() {

    return m_settings;

}

QStringList ConfigManager::musicDatabasePaths() {
    QString libraryFolderPath = m_settings->value("Common/LibraryPath/").toString();

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

QString ConfigManager::GetConfigPath(){

    // 检查用户目录下的IntMusic/config.ini
    QString userConfigPath = GetHomeConfigPath();
    if (QFileInfo::exists(userConfigPath)) {
        return userConfigPath;
    }

    // 检查音乐目录下的IntMusic/config.ini
    QString musicConfigPath = GetMusicFolderConfigPath();
    if (QFileInfo::exists(musicConfigPath)) {
        return musicConfigPath;
    }

    // 两个路径都不存在创建新Config
    QString newConfigPath = SaveConfig();

    if (QFileInfo::exists(newConfigPath)) {
        return newConfigPath;
    } else {
        return QString();
    }

}

QString ConfigManager::GetHomePath(){
    QDir userDir(QDir::home());
    return userDir.filePath("IntMusic/");
}

QString ConfigManager::GetMusicFolderPath(){
    QString musicBaseDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    return QDir(musicBaseDir).filePath("IntMusic/");
}

QString ConfigManager::GetHomeConfigPath(){
    QDir userDir(QDir::home());
    QString userConfigPath = userDir.filePath("IntMusic/" + configFileName);
    return userConfigPath;
}

QString ConfigManager::GetMusicFolderConfigPath(){
    QString musicBaseDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    QString musicConfigPath = QDir(musicBaseDir).filePath("IntMusic/" + configFileName);
    return musicConfigPath;
}

QString ConfigManager::GetHomeLibraryPath(){
    QDir userDir(QDir::home());
    QString userConfigPath = userDir.filePath("IntMusic/library/");
    return userConfigPath;
}
QString ConfigManager::GetMusicFolderLibraryPath(){
    QString musicBaseDir = QStandardPaths::writableLocation(QStandardPaths::MusicLocation);
    QString musicConfigPath = QDir(musicBaseDir).filePath("IntMusic/library/");
    return musicConfigPath;
}

QString ConfigManager::SaveConfig(QString configPath, QSettings* externalSettings) {

    if (configPath.isEmpty()) {
        configPath = GetHomeConfigPath();
    }

    // 创建/打开配置文件
    QSettings targetSettings(configPath, QSettings::IniFormat);


    // 如果传入了外部配置，复制其内容
    if (externalSettings) {
        // 确保读取到最新内容
        externalSettings->sync();

        // 复制所有键值对
        foreach (const QString &key, externalSettings->allKeys()) {
            targetSettings.setValue(key, externalSettings->value(key));
        }
    }

    // 强制写入磁盘
    targetSettings.sync();
    return configPath;
}

int ConfigManager::setConfig(QString key, QVariant value) {

    if (!m_settings) {
        qWarning() << "m_settings is null!";
        return -1; // 返回错误码
    }

    // 检查 key 是否存在
    if (m_settings->contains(key)) {
        QVariant oldValue = m_settings->value(key);
        qDebug() << "Key" << key << "exists. Old value:" << oldValue << "New value:" << value;
    } else {
        qDebug() << "Key" << key << "does not exist. Setting new value:" << value;
    }

    // 设置新的值
    m_settings->setValue(key, value);
    m_settings->sync(); // 强制写入磁盘

    // 发射配置变化信号
    emit settingsChanged();

    return 1;
}
