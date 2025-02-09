#include "settingcommon.h"

SettingCommon::SettingCommon(QWidget *parent)
    : QWidget{parent},configManager(ConfigManager::instance())
{

    QVBoxLayout *commonLayout = new QVBoxLayout(this);

    commonLayout->setContentsMargins(10, 10, 10, 10);
    commonLayout->setSpacing(10);

    // 配置文件存储路径
    CardCombo *configStoragePathCard = new CardCombo("数据存储路径","选择数据存储的位置", this);
    QStringList configPathOptions;
    configPathOptions << "用户数据路径" << "音乐目录";
    configStoragePathCard->setOptions(configPathOptions);
    QString currentConfigPath = configManager.getSettings()->value("Common/ConfigPath").toString();
    if (currentConfigPath == configManager.GetHomeConfigPath()) {
        configStoragePathCard->setCurrentIndex(0); // 选中"用户数据路径"
    } else if (currentConfigPath == configManager.GetMusicFolderConfigPath()) {
        configStoragePathCard->setCurrentIndex(1); // 选中"音乐目录"
    } else {
        configStoragePathCard->setCurrentIndex(0);
    }

    // 库文件存储路径
    CardCombo *libraryStoragePathCard = new CardCombo("数据存储路径","选择数据存储的位置", this);
    QStringList libraryPathOptions;
    libraryPathOptions << "用户数据路径" << "音乐目录";
    libraryStoragePathCard->setOptions(libraryPathOptions);
    QString currentLibraryPath = configManager.getSettings()->value("Common/LibraryPath").toString();
    if (currentLibraryPath == configManager.GetHomeLibraryPath()) {
        libraryStoragePathCard->setCurrentIndex(0); // 选中"用户数据路径"
    } else if (currentLibraryPath == configManager.GetMusicFolderLibraryPath()) {
        libraryStoragePathCard->setCurrentIndex(1); // 选中"音乐目录"
    } else {
        libraryStoragePathCard->setCurrentIndex(0);
    }

    // Test Button
    testButton = new CardButton("Test", "测试向数据库中添加一首歌", this);


    // 将 ComboCard 添加到布局
    commonLayout->addWidget(configStoragePathCard);
    commonLayout->addWidget(libraryStoragePathCard);
    commonLayout->addWidget(testButton);
    commonLayout->addStretch();



    connect(configStoragePathCard->getComboBox(),
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SettingCommon::ConfigStoragePathChanged
    );
    connect(libraryStoragePathCard->getComboBox(),
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SettingCommon::LibraryStoragePathChanged
            );
    connect(testButton->getPushButton(),
            &QPushButton::clicked,
            this,
            [this]{
                MusicLibrary::SongInfo song;
                song.title = "My Song";
                song.durationMs = 240000;
                song.fileSize = 10240; // KB
                song.filePath = "/music/song1.mp3";
                song.albumId = -1;
                song.albumName = "Test";
                song.publisher = "Test Publisher";
                song.albumVersion = "银圈";
                song.releaseTime = QDateTime::currentDateTime();
                song.albumReleaseTime = QDateTime::currentDateTime();
                song.totalTracks = 0;
                song.totalDiscs = 0;
                song.trackNumber = 1;
                song.discNumber = 1;
                song.isFavorite = false;
                song.isLive = false;
                song.isSingle = false;
                song.language = {"zh","en"};
                song.liveEvent = "Concert";
                song.liveEventCount = "Taipei";
                song.isCover = false;
                song.isMashup = false;
                song.characters.insert("performer", "John Doe");
                song.characters.insert("composer", "Jane Smith");
                song.genres = {"Pop", "Rock"};
                song.languages = {"en", "zh"};

                if (MusicLibrary::instance().insertSong(song)) {
                    qDebug() << "Song inserted successfully";
                } else {
                    qDebug() << "Failed to insert song";
                }

            });
}


void SettingCommon::ConfigStoragePathChanged(int index){
    QString path;
    switch (index) {
    case 0: // "用户数据路径"
        path = configManager.GetHomeConfigPath();
        break;
    case 1: // "音乐目录"
        path = configManager.GetMusicFolderConfigPath();
        break;
    default:
        qWarning() << "Invalid index:" << index;
        return;
    }
    configManager.setConfig("Common/ConfigPath", path);
}

void SettingCommon::LibraryStoragePathChanged(int index){
    QString path;
    switch (index) {
    case 0: // "用户数据路径"
        path = configManager.GetHomeLibraryPath();
        break;
    case 1: // "音乐目录"
        path = configManager.GetMusicFolderLibraryPath();
        break;
    default:
        qWarning() << "Invalid index:" << index;
        return;
    }
    configManager.setConfig("Common/LibraryPath", path);
}
