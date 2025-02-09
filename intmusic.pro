QT       += core gui multimedia sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    app/custom/CardButton/cardbutton.cpp \
    app/custom/CardCombo/cardcombo.cpp \
    app/custom/FlexibleTabWidget/flexibletabwidget.cpp \
    app/custom/MainSubTitle/mainsubtitle.cpp \
    app/util/Config/configmanager.cpp \
    app/util/Config/musiclibrary.cpp \
    app/util/MusicDB/musicscanner.cpp \
    app/util/MusicPlayer/musicplayer.cpp \
    app/view/IntMusicIndexTab/index.cpp \
    app/view/IntMusicIndexTab/intmusicindextab.cpp \
    app/view/IntMusicIndexTab/musiclist.cpp \
    app/view/IntMusicIndexTab/setting.cpp \
    app/view/MiniPlayer/miniplayer.cpp \
    app/view/MusicListView/musiclistview.cpp \
    app/view/Setting/settingcommon.cpp \
    app/view/Setting/settingconfig.cpp \
    main.cpp \
    intmusic.cpp

HEADERS += \
    app/custom/CardButton/cardbutton.h \
    app/custom/CardCombo/cardcombo.h \
    app/custom/FlexibleTabWidget/flexibletabwidget.h \
    app/custom/MainSubTitle/mainsubtitle.h \
    app/util/Config/configmanager.h \
    app/util/Config/musiclibrary.h \
    app/util/MusicDB/musicscanner.h \
    app/util/MusicPlayer/musicplayer.h \
    app/view/IntMusicIndexTab/index.h \
    app/view/IntMusicIndexTab/intmusicindextab.h \
    app/view/IntMusicIndexTab/musiclist.h \
    app/view/IntMusicIndexTab/setting.h \
    app/view/MiniPlayer/miniplayer.h \
    app/view/MusicListView/musiclistview.h \
    app/view/Setting/settingcommon.h \
    app/view/Setting/settingconfig.h \
    intmusic.h

# FORMS += \
#     intmusic.ui

TRANSLATIONS += \
    intmusic_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
