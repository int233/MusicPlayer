#ifndef MUSICLISTVIEW_H
#define MUSICLISTVIEW_H

#include <QWidget>
#include <QListView>
#include <QStandardItemModel>
#include <QVBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QMessageBox>
#include <QPushButton>

#include "app/util/Config/musiclibrary.h"

class MusicListView : public QWidget {
    Q_OBJECT

public:
    explicit MusicListView(QWidget *parent = nullptr);
    void refreshMusicList();

private:
    void setupUI();
    void loadMusicData();

    QListView *listView;
    QStandardItemModel *model;
    MusicLibrary &musicLib;

    QPushButton *refreshButton;

private slots:
    void onItemClicked(const QModelIndex &index);
};

#endif // MUSICLISTVIEW_H
