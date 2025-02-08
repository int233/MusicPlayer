# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'intmusic.ui'
##
## Created by: Qt User Interface Compiler version 6.8.1
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QAction, QBrush, QColor, QConicalGradient,
    QCursor, QFont, QFontDatabase, QGradient,
    QIcon, QImage, QKeySequence, QLinearGradient,
    QPainter, QPalette, QPixmap, QRadialGradient,
    QTransform)
from PySide6.QtWidgets import (QAbstractSpinBox, QApplication, QFrame, QGraphicsView,
    QHBoxLayout, QLabel, QMainWindow, QMenu,
    QMenuBar, QPushButton, QScrollArea, QSizePolicy,
    QSlider, QStatusBar, QTabWidget, QTimeEdit,
    QVBoxLayout, QWidget)

class Ui_IntMusic(object):
    def setupUi(self, IntMusic):
        if not IntMusic.objectName():
            IntMusic.setObjectName(u"IntMusic")
        IntMusic.resize(1042, 718)
        self.actionSetting = QAction(IntMusic)
        self.actionSetting.setObjectName(u"actionSetting")
        self.actionPause = QAction(IntMusic)
        self.actionPause.setObjectName(u"actionPause")
        self.actionStart = QAction(IntMusic)
        self.actionStart.setObjectName(u"actionStart")
        self.actionNext = QAction(IntMusic)
        self.actionNext.setObjectName(u"actionNext")
        self.actionPrevious = QAction(IntMusic)
        self.actionPrevious.setObjectName(u"actionPrevious")
        self.centralwidget = QWidget(IntMusic)
        self.centralwidget.setObjectName(u"centralwidget")
        sizePolicy = QSizePolicy(QSizePolicy.Policy.Expanding, QSizePolicy.Policy.Expanding)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.centralwidget.sizePolicy().hasHeightForWidth())
        self.centralwidget.setSizePolicy(sizePolicy)
        self.verticalLayout = QVBoxLayout(self.centralwidget)
        self.verticalLayout.setObjectName(u"verticalLayout")
        self.MainWindows = QTabWidget(self.centralwidget)
        self.MainWindows.setObjectName(u"MainWindows")
        self.MainWindows.setTabPosition(QTabWidget.TabPosition.West)
        self.MainWindows.setTabShape(QTabWidget.TabShape.Triangular)
        self.MainWindows.setElideMode(Qt.TextElideMode.ElideLeft)
        self.MainWindows.setUsesScrollButtons(True)
        self.Index = QWidget()
        self.Index.setObjectName(u"Index")
        self.verticalLayout_2 = QVBoxLayout(self.Index)
        self.verticalLayout_2.setObjectName(u"verticalLayout_2")
        self.IndexScrollContent = QScrollArea(self.Index)
        self.IndexScrollContent.setObjectName(u"IndexScrollContent")
        self.IndexScrollContent.setWidgetResizable(True)
        self.IndexContent = QWidget()
        self.IndexContent.setObjectName(u"IndexContent")
        self.IndexContent.setGeometry(QRect(0, 0, 947, 453))
        self.verticalLayout_3 = QVBoxLayout(self.IndexContent)
        self.verticalLayout_3.setObjectName(u"verticalLayout_3")
        self.MostPlayed = QWidget(self.IndexContent)
        self.MostPlayed.setObjectName(u"MostPlayed")
        self.verticalLayout_4 = QVBoxLayout(self.MostPlayed)
        self.verticalLayout_4.setObjectName(u"verticalLayout_4")
        self.MostPlayedTitle = QLabel(self.MostPlayed)
        self.MostPlayedTitle.setObjectName(u"MostPlayedTitle")

        self.verticalLayout_4.addWidget(self.MostPlayedTitle)

        self.MostPlayedCards = QScrollArea(self.MostPlayed)
        self.MostPlayedCards.setObjectName(u"MostPlayedCards")
        self.MostPlayedCards.setWidgetResizable(True)
        self.MostPlayedCard = QWidget()
        self.MostPlayedCard.setObjectName(u"MostPlayedCard")
        self.MostPlayedCard.setGeometry(QRect(0, 0, 897, 160))
        self.MostPlayedCards.setWidget(self.MostPlayedCard)

        self.verticalLayout_4.addWidget(self.MostPlayedCards)


        self.verticalLayout_3.addWidget(self.MostPlayed)

        self.ResentlyAdded = QWidget(self.IndexContent)
        self.ResentlyAdded.setObjectName(u"ResentlyAdded")
        self.verticalLayout_5 = QVBoxLayout(self.ResentlyAdded)
        self.verticalLayout_5.setObjectName(u"verticalLayout_5")
        self.ResentlyAddedTitle = QLabel(self.ResentlyAdded)
        self.ResentlyAddedTitle.setObjectName(u"ResentlyAddedTitle")

        self.verticalLayout_5.addWidget(self.ResentlyAddedTitle)

        self.ResentlyAddedCards = QScrollArea(self.ResentlyAdded)
        self.ResentlyAddedCards.setObjectName(u"ResentlyAddedCards")
        self.ResentlyAddedCards.setWidgetResizable(True)
        self.ResentlyAddedCard = QWidget()
        self.ResentlyAddedCard.setObjectName(u"ResentlyAddedCard")
        self.ResentlyAddedCard.setGeometry(QRect(0, 0, 897, 159))
        self.ResentlyAddedCards.setWidget(self.ResentlyAddedCard)

        self.verticalLayout_5.addWidget(self.ResentlyAddedCards)


        self.verticalLayout_3.addWidget(self.ResentlyAdded)

        self.IndexScrollContent.setWidget(self.IndexContent)

        self.verticalLayout_2.addWidget(self.IndexScrollContent)

        self.line = QFrame(self.Index)
        self.line.setObjectName(u"line")
        self.line.setFrameShape(QFrame.Shape.VLine)
        self.line.setFrameShadow(QFrame.Shadow.Sunken)

        self.verticalLayout_2.addWidget(self.line)

        self.MainWindows.addTab(self.Index, "")
        self.Music = QWidget()
        self.Music.setObjectName(u"Music")
        self.MainWindows.addTab(self.Music, "")

        self.verticalLayout.addWidget(self.MainWindows)

        self.MiniPlayerControl = QWidget(self.centralwidget)
        self.MiniPlayerControl.setObjectName(u"MiniPlayerControl")
        sizePolicy1 = QSizePolicy(QSizePolicy.Policy.Preferred, QSizePolicy.Policy.Preferred)
        sizePolicy1.setHorizontalStretch(0)
        sizePolicy1.setVerticalStretch(17)
        sizePolicy1.setHeightForWidth(self.MiniPlayerControl.sizePolicy().hasHeightForWidth())
        self.MiniPlayerControl.setSizePolicy(sizePolicy1)
        self.horizontalLayout_2 = QHBoxLayout(self.MiniPlayerControl)
        self.horizontalLayout_2.setObjectName(u"horizontalLayout_2")
        self.MiniAlbum = QWidget(self.MiniPlayerControl)
        self.MiniAlbum.setObjectName(u"MiniAlbum")
        self.horizontalLayout = QHBoxLayout(self.MiniAlbum)
        self.horizontalLayout.setObjectName(u"horizontalLayout")
        self.AlbumImage = QGraphicsView(self.MiniAlbum)
        self.AlbumImage.setObjectName(u"AlbumImage")

        self.horizontalLayout.addWidget(self.AlbumImage)

        self.MusicTitles = QWidget(self.MiniAlbum)
        self.MusicTitles.setObjectName(u"MusicTitles")
        self.verticalLayout_6 = QVBoxLayout(self.MusicTitles)
        self.verticalLayout_6.setObjectName(u"verticalLayout_6")
        self.PlayingMusicTitle = QLabel(self.MusicTitles)
        self.PlayingMusicTitle.setObjectName(u"PlayingMusicTitle")

        self.verticalLayout_6.addWidget(self.PlayingMusicTitle)

        self.PlayingAblumTitle = QLabel(self.MusicTitles)
        self.PlayingAblumTitle.setObjectName(u"PlayingAblumTitle")

        self.verticalLayout_6.addWidget(self.PlayingAblumTitle)


        self.horizontalLayout.addWidget(self.MusicTitles)


        self.horizontalLayout_2.addWidget(self.MiniAlbum)

        self.MiniSlider = QWidget(self.MiniPlayerControl)
        self.MiniSlider.setObjectName(u"MiniSlider")
        self.horizontalLayout_4 = QHBoxLayout(self.MiniSlider)
        self.horizontalLayout_4.setObjectName(u"horizontalLayout_4")
        self.MiniPlayerCurrentTime = QTimeEdit(self.MiniSlider)
        self.MiniPlayerCurrentTime.setObjectName(u"MiniPlayerCurrentTime")
        self.MiniPlayerCurrentTime.setButtonSymbols(QAbstractSpinBox.ButtonSymbols.NoButtons)

        self.horizontalLayout_4.addWidget(self.MiniPlayerCurrentTime)

        self.MiniPlayerTimeAxis = QWidget(self.MiniSlider)
        self.MiniPlayerTimeAxis.setObjectName(u"MiniPlayerTimeAxis")
        self.verticalLayout_7 = QVBoxLayout(self.MiniPlayerTimeAxis)
        self.verticalLayout_7.setObjectName(u"verticalLayout_7")
        self.MiniPlayerProgress = QSlider(self.MiniPlayerTimeAxis)
        self.MiniPlayerProgress.setObjectName(u"MiniPlayerProgress")
        self.MiniPlayerProgress.setOrientation(Qt.Orientation.Horizontal)

        self.verticalLayout_7.addWidget(self.MiniPlayerProgress)


        self.horizontalLayout_4.addWidget(self.MiniPlayerTimeAxis)

        self.MiniPlayerTotalTime = QTimeEdit(self.MiniSlider)
        self.MiniPlayerTotalTime.setObjectName(u"MiniPlayerTotalTime")
        self.MiniPlayerTotalTime.setFrame(False)
        self.MiniPlayerTotalTime.setButtonSymbols(QAbstractSpinBox.ButtonSymbols.NoButtons)

        self.horizontalLayout_4.addWidget(self.MiniPlayerTotalTime)

        self.horizontalLayout_4.setStretch(0, 1)
        self.horizontalLayout_4.setStretch(1, 8)
        self.horizontalLayout_4.setStretch(2, 1)

        self.horizontalLayout_2.addWidget(self.MiniSlider)

        self.MiniControler = QWidget(self.MiniPlayerControl)
        self.MiniControler.setObjectName(u"MiniControler")
        sizePolicy2 = QSizePolicy(QSizePolicy.Policy.Maximum, QSizePolicy.Policy.Maximum)
        sizePolicy2.setHorizontalStretch(20)
        sizePolicy2.setVerticalStretch(0)
        sizePolicy2.setHeightForWidth(self.MiniControler.sizePolicy().hasHeightForWidth())
        self.MiniControler.setSizePolicy(sizePolicy2)
        self.horizontalLayout_3 = QHBoxLayout(self.MiniControler)
        self.horizontalLayout_3.setSpacing(0)
        self.horizontalLayout_3.setObjectName(u"horizontalLayout_3")
        self.horizontalLayout_3.setContentsMargins(0, 0, 0, 0)
        self.MiniPlayerOrderMode = QPushButton(self.MiniControler)
        self.MiniPlayerOrderMode.setObjectName(u"MiniPlayerOrderMode")
        sizePolicy3 = QSizePolicy(QSizePolicy.Policy.Fixed, QSizePolicy.Policy.Fixed)
        sizePolicy3.setHorizontalStretch(20)
        sizePolicy3.setVerticalStretch(20)
        sizePolicy3.setHeightForWidth(self.MiniPlayerOrderMode.sizePolicy().hasHeightForWidth())
        self.MiniPlayerOrderMode.setSizePolicy(sizePolicy3)
        icon = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.SyncSynchronizing))
        self.MiniPlayerOrderMode.setIcon(icon)

        self.horizontalLayout_3.addWidget(self.MiniPlayerOrderMode)

        self.MiniPlayerPrevious = QPushButton(self.MiniControler)
        self.MiniPlayerPrevious.setObjectName(u"MiniPlayerPrevious")
        sizePolicy3.setHeightForWidth(self.MiniPlayerPrevious.sizePolicy().hasHeightForWidth())
        self.MiniPlayerPrevious.setSizePolicy(sizePolicy3)
        icon1 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.MediaSkipBackward))
        self.MiniPlayerPrevious.setIcon(icon1)

        self.horizontalLayout_3.addWidget(self.MiniPlayerPrevious)

        self.MiniPlayerPause = QPushButton(self.MiniControler)
        self.MiniPlayerPause.setObjectName(u"MiniPlayerPause")
        icon2 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.MediaPlaybackPause))
        self.MiniPlayerPause.setIcon(icon2)

        self.horizontalLayout_3.addWidget(self.MiniPlayerPause)

        self.MiniPlayerNext = QPushButton(self.MiniControler)
        self.MiniPlayerNext.setObjectName(u"MiniPlayerNext")
        icon3 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.MediaSkipForward))
        self.MiniPlayerNext.setIcon(icon3)

        self.horizontalLayout_3.addWidget(self.MiniPlayerNext)

        self.MiniPlayerSound = QPushButton(self.MiniControler)
        self.MiniPlayerSound.setObjectName(u"MiniPlayerSound")
        icon4 = QIcon(QIcon.fromTheme(QIcon.ThemeIcon.AudioVolumeLow))
        self.MiniPlayerSound.setIcon(icon4)

        self.horizontalLayout_3.addWidget(self.MiniPlayerSound)


        self.horizontalLayout_2.addWidget(self.MiniControler)

        self.horizontalLayout_2.setStretch(0, 1)
        self.horizontalLayout_2.setStretch(1, 7)
        self.horizontalLayout_2.setStretch(2, 2)

        self.verticalLayout.addWidget(self.MiniPlayerControl)

        self.verticalLayout.setStretch(0, 9)
        self.verticalLayout.setStretch(1, 1)
        IntMusic.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(IntMusic)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 1042, 24))
        self.menuIntMusic = QMenu(self.menubar)
        self.menuIntMusic.setObjectName(u"menuIntMusic")
        self.menu = QMenu(self.menubar)
        self.menu.setObjectName(u"menu")
        IntMusic.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(IntMusic)
        self.statusbar.setObjectName(u"statusbar")
        IntMusic.setStatusBar(self.statusbar)

        self.menubar.addAction(self.menuIntMusic.menuAction())
        self.menubar.addAction(self.menu.menuAction())
        self.menuIntMusic.addAction(self.actionSetting)
        self.menu.addAction(self.actionPause)
        self.menu.addAction(self.actionStart)
        self.menu.addAction(self.actionNext)
        self.menu.addAction(self.actionPrevious)

        self.retranslateUi(IntMusic)

        self.MainWindows.setCurrentIndex(0)


        QMetaObject.connectSlotsByName(IntMusic)
    # setupUi

    def retranslateUi(self, IntMusic):
        IntMusic.setWindowTitle(QCoreApplication.translate("IntMusic", u"IntMusic", None))
        self.actionSetting.setText(QCoreApplication.translate("IntMusic", u"Setting", None))
        self.actionPause.setText(QCoreApplication.translate("IntMusic", u"Pause", None))
        self.actionStart.setText(QCoreApplication.translate("IntMusic", u"Start", None))
        self.actionNext.setText(QCoreApplication.translate("IntMusic", u"Next", None))
        self.actionPrevious.setText(QCoreApplication.translate("IntMusic", u"Previous", None))
        self.MostPlayedTitle.setText(QCoreApplication.translate("IntMusic", u"\u6700\u591a\u64ad\u653e", None))
        self.ResentlyAddedTitle.setText(QCoreApplication.translate("IntMusic", u"\u6700\u8fd1\u6dfb\u52a0", None))
        self.MainWindows.setTabText(self.MainWindows.indexOf(self.Index), QCoreApplication.translate("IntMusic", u"\u9996\u9875", None))
        self.MainWindows.setTabText(self.MainWindows.indexOf(self.Music), QCoreApplication.translate("IntMusic", u"\u97f3\u4e50", None))
        self.PlayingMusicTitle.setText(QCoreApplication.translate("IntMusic", u"TextLabel", None))
        self.PlayingAblumTitle.setText(QCoreApplication.translate("IntMusic", u"TextLabel", None))
        self.MiniPlayerOrderMode.setText("")
        self.MiniPlayerPrevious.setText("")
        self.MiniPlayerPause.setText("")
        self.MiniPlayerNext.setText("")
        self.MiniPlayerSound.setText("")
        self.menuIntMusic.setTitle(QCoreApplication.translate("IntMusic", u"IntMusic", None))
        self.menu.setTitle(QCoreApplication.translate("IntMusic", u"\u63a7\u5236", None))
    # retranslateUi

