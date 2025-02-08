#ifndef SETTINGCOMMON_H
#define SETTINGCOMMON_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>

#include "app/custom/CardCombo/cardcombo.h"
#include "app/util/Config/configmanager.h"


class SettingCommon : public QWidget
{
    Q_OBJECT
public:
    explicit SettingCommon(QWidget *parent = nullptr);

private:

    QVBoxLayout *commonLayout;
    CardCombo *configStoragePathCard;
    ConfigManager &configManager;

private slots:
    void ConfigStoragePathChanged(int index);
    void LibraryStoragePathChanged(int index);
};

#endif // SETTINGCOMMON_H
