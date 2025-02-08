#include "cardcombo.h"

CardCombo::CardCombo(const QString &title, const QString &subtitle, QWidget *parent)
    : QWidget{parent}
{
    // 主布局
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(5);

    // 标题
    titleLabel = new QLabel(title, this);
    titleLabel->setStyleSheet("font-size: 16px; font-weight: bold;");

    // 副标题
    subtitleLabel = new QLabel(subtitle, this);
    subtitleLabel->setStyleSheet("font-size: 12px; color: gray;");

    // 组合框
    comboBox = new QComboBox(this);
    comboBox->setStyleSheet("QComboBox { padding: 5px; }");

    // 添加到布局
    layout->addWidget(titleLabel);
    layout->addWidget(subtitleLabel);
    layout->addWidget(comboBox);
}

QString CardCombo::currentSelection() const {
    return comboBox->currentText();
}

void CardCombo::setOptions(const QStringList &options) {
    comboBox->clear();
    comboBox->addItems(options);
}

void CardCombo::setCurrentIndex(int index){
    comboBox->setCurrentIndex(index);
}
