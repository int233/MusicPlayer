#ifndef MUSICLISTVIEW_H
#define MUSICLISTVIEW_H

#include <QWidget>

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class MusicListView : public QWidget {
    Q_OBJECT  // 使用 Q_OBJECT 宏以支持信号和槽机制

public:
    explicit MusicListView(QWidget *parent = nullptr); // 构造函数声明
    ~MusicListView() override = default; // 析构函数声明，如果需要

private:
    // 可以在这里声明界面组件，例如标签、按钮等
    QLabel *label;
    QPushButton *button;
    QVBoxLayout *layout;
};

#endif // MUSICLISTVIEW_H
