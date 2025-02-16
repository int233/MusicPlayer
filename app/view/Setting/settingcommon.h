#ifndef SETTINGCOMMON_H
#define SETTINGCOMMON_H

#include <QWidget>
#include <QVBoxLayout>
#include <QGroupBox>
#include <QRadioButton>

#include "app/custom/CardCombo/cardcombo.h"
#include "app/custom/CardButton/cardbutton.h"
#include "app/custom/CardPathSelect/cardpathselect.h"

#include "app/util/Config/configmanager.h"
#include "app/util/Config/musiclibrary.h"
#include "app/util/MusicDB/musicscanner.h"

class SettingCommon : public QWidget
{
    Q_OBJECT
public:
    explicit SettingCommon(QWidget *parent = nullptr);

private:

    QVBoxLayout *commonLayout;
    CardCombo *configStoragePathCard;
    CardCombo *libraryStoragePathCard;
    CardButton *testButton;
    CardPathSelect *testPathSelect;
    CardButton *testScannerButton;
    ConfigManager &configManager;
    MusicScanner &musicScanner;

private slots:
    void ConfigStoragePathChanged(int index);
    void LibraryStoragePathChanged(int index);
};

#endif // SETTINGCOMMON_H
