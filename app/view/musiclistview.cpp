#include "musiclistview.h"

MusicListView::MusicListView(QWidget *parent) : QWidget(parent) {
    label = new QLabel(this);    // 创建 QLabel 实例
    label->setText("音乐");       // 设置 QLabel 文本为 "音乐"

    // 创建一个布局并添加 QLabel
    QVBoxLayout *layout = new QVBoxLayout(this);  // 创建 QVBoxLayout
    layout->addWidget(label);  // 将 QLabel 添加到布局中

    setLayout(layout);  // 将布局设置为 MusicListView 的布局
}
