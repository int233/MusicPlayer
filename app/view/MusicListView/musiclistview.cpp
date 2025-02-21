#include "musiclistview.h"
#include "app/util/Config/musicdatastruct.h"

MusicListView::MusicListView(QWidget *parent)
    : QWidget(parent), musicLib(MusicLibrary::instance()),plManager(PlayListManager::instance())
{
    setupUI();
    loadMusicData();
}

void MusicListView::setupUI()
{
    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    // 标题
    QLabel *titleLabel = new QLabel("音乐", this);
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font-size: 18px; font-weight: bold; margin: 10px;");

    // 列表视图
    listView = new QListView(this);
    model = new QStandardItemModel(this);
    listView->setModel(model);
    listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    // 刷新按钮
    QPushButton *refreshButton = new QPushButton("刷新", this);
    connect(refreshButton, &QPushButton::clicked, this, &MusicListView::refreshMusicList);

    // 布局
    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(listView);
    mainLayout->addWidget(refreshButton);

    // 连接点击信号
    connect(listView, &QListView::clicked, this, &MusicListView::onItemClicked);
}

void MusicListView::loadMusicData()
{
    model->clear();

    QString debugTitle = "loadMusicData";

    qDebug() << debugTitle << ";";

    try {
        // 获取所有歌曲信息
        PlayListInfo allSongsPlayListInfo = plManager.GetPlayList();
        QHash<int, SongInfo> allSongs = allSongsPlayListInfo.playlist->getSongs();

        for(const SongInfo& song : allSongs){
            qDebug() << debugTitle << " - Title: " << song.title << ", album ID: "<< song.albumID << ", 专辑演出者: " << song.artists.first() << ", Path: " <<song.filePath;\
            QStandardItem *item = new QStandardItem();
            QString displayText = QString("%1\n艺术家: %2\n时长: %3")
                                                .arg(song.title)
                                                .arg(song.artists.join(", "))
                                                .arg(QTime::fromMSecsSinceStartOfDay(song.durations*1000).toString("mm:ss"));
            item->setText(displayText);
            item->setData(song.filePath, Qt::UserRole); // 存储文件路径

            model->appendRow(item);
        }

        // for (const auto &song : songs) {
        //     QStandardItem *item = new QStandardItem();

        //     // 格式化显示信息
        //     QString displayText = QString("%1\n艺术家: %2\n时长: %3")
        //                               .arg(song.title)
        //                               .arg(song.artists.join(", "))
        //                               .arg(QTime::fromMSecsSinceStartOfDay(song.duration*1000).toString("mm:ss"));

        //     item->setText(displayText);
        //     item->setData(song.filePath, Qt::UserRole); // 存储文件路径

        //     model->appendRow(item);
        // }
    } catch (const std::exception &e) {
        QMessageBox::critical(this, "错误", QString("加载音乐数据失败: %1").arg(e.what()));
    }
}

void MusicListView::refreshMusicList()
{
    loadMusicData();
}

void MusicListView::onItemClicked(const QModelIndex &index)
{
    QString filePath = index.data(Qt::UserRole).toString();
    qDebug() << "选中歌曲路径:" << filePath;
    

}

