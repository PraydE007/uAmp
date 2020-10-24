/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.15.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QProgressBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *albumImage;
    QVBoxLayout *verticalLayout_2;
    QLabel *songNameLabel;
    QLabel *fileLabel;
    QHBoxLayout *controlLayout;
    QSlider *volumeSlider;
    QSpacerItem *horizontalSpacer;
    QHBoxLayout *horizontalLayout_2;
    QPushButton *stopSongBtn;
    QPushButton *previousSongBtn;
    QPushButton *playSongBtn;
    QPushButton *pauseSongBtn;
    QPushButton *nextSongBtn;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *repeatBtn;
    QProgressBar *progressBar;
    QListView *playlist;
    QHBoxLayout *playlistControlLayout;
    QPushButton *loadPlaylistBtn;
    QSpacerItem *horizontalSpacer_3;
    QPushButton *addSongBtn;
    QPushButton *popSongBtn;
    QMenuBar *menubar;
    QMenu *menuMenu;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(600, 800);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        albumImage = new QLabel(centralwidget);
        albumImage->setObjectName(QString::fromUtf8("albumImage"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(albumImage->sizePolicy().hasHeightForWidth());
        albumImage->setSizePolicy(sizePolicy);
        albumImage->setMinimumSize(QSize(100, 100));
        albumImage->setMaximumSize(QSize(100, 100));
        albumImage->setPixmap(QPixmap(QString::fromUtf8("../res/images/empty-image.png")));
        albumImage->setScaledContents(true);

        horizontalLayout->addWidget(albumImage);

        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(6, -1, -1, -1);
        songNameLabel = new QLabel(centralwidget);
        songNameLabel->setObjectName(QString::fromUtf8("songNameLabel"));
        QSizePolicy sizePolicy1(QSizePolicy::Minimum, QSizePolicy::Maximum);
        sizePolicy1.setHorizontalStretch(0);
        sizePolicy1.setVerticalStretch(0);
        sizePolicy1.setHeightForWidth(songNameLabel->sizePolicy().hasHeightForWidth());
        songNameLabel->setSizePolicy(sizePolicy1);
        songNameLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(songNameLabel);

        fileLabel = new QLabel(centralwidget);
        fileLabel->setObjectName(QString::fromUtf8("fileLabel"));
        sizePolicy1.setHeightForWidth(fileLabel->sizePolicy().hasHeightForWidth());
        fileLabel->setSizePolicy(sizePolicy1);
        fileLabel->setAlignment(Qt::AlignCenter);

        verticalLayout_2->addWidget(fileLabel);


        horizontalLayout->addLayout(verticalLayout_2);


        verticalLayout->addLayout(horizontalLayout);

        controlLayout = new QHBoxLayout();
        controlLayout->setSpacing(15);
        controlLayout->setObjectName(QString::fromUtf8("controlLayout"));
        controlLayout->setSizeConstraint(QLayout::SetDefaultConstraint);
        controlLayout->setContentsMargins(5, -1, 5, -1);
        volumeSlider = new QSlider(centralwidget);
        volumeSlider->setObjectName(QString::fromUtf8("volumeSlider"));
        QSizePolicy sizePolicy2(QSizePolicy::Preferred, QSizePolicy::Minimum);
        sizePolicy2.setHorizontalStretch(0);
        sizePolicy2.setVerticalStretch(0);
        sizePolicy2.setHeightForWidth(volumeSlider->sizePolicy().hasHeightForWidth());
        volumeSlider->setSizePolicy(sizePolicy2);
        volumeSlider->setMinimumSize(QSize(50, 0));
        volumeSlider->setMaximumSize(QSize(100, 16777215));
        volumeSlider->setMaximum(100);
        volumeSlider->setOrientation(Qt::Horizontal);

        controlLayout->addWidget(volumeSlider);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        controlLayout->addItem(horizontalSpacer);

        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        stopSongBtn = new QPushButton(centralwidget);
        stopSongBtn->setObjectName(QString::fromUtf8("stopSongBtn"));
        sizePolicy.setHeightForWidth(stopSongBtn->sizePolicy().hasHeightForWidth());
        stopSongBtn->setSizePolicy(sizePolicy);
        stopSongBtn->setMinimumSize(QSize(50, 50));
        stopSongBtn->setMaximumSize(QSize(50, 50));
        stopSongBtn->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(stopSongBtn);

        previousSongBtn = new QPushButton(centralwidget);
        previousSongBtn->setObjectName(QString::fromUtf8("previousSongBtn"));
        sizePolicy.setHeightForWidth(previousSongBtn->sizePolicy().hasHeightForWidth());
        previousSongBtn->setSizePolicy(sizePolicy);
        previousSongBtn->setMinimumSize(QSize(50, 50));
        previousSongBtn->setMaximumSize(QSize(50, 50));
        previousSongBtn->setIconSize(QSize(32, 32));
        previousSongBtn->setFlat(false);

        horizontalLayout_2->addWidget(previousSongBtn);

        playSongBtn = new QPushButton(centralwidget);
        playSongBtn->setObjectName(QString::fromUtf8("playSongBtn"));
        sizePolicy.setHeightForWidth(playSongBtn->sizePolicy().hasHeightForWidth());
        playSongBtn->setSizePolicy(sizePolicy);
        playSongBtn->setMinimumSize(QSize(60, 60));
        playSongBtn->setMaximumSize(QSize(60, 60));
        playSongBtn->setIconSize(QSize(40, 40));
        playSongBtn->setFlat(false);

        horizontalLayout_2->addWidget(playSongBtn);

        pauseSongBtn = new QPushButton(centralwidget);
        pauseSongBtn->setObjectName(QString::fromUtf8("pauseSongBtn"));
        sizePolicy.setHeightForWidth(pauseSongBtn->sizePolicy().hasHeightForWidth());
        pauseSongBtn->setSizePolicy(sizePolicy);
        pauseSongBtn->setMinimumSize(QSize(50, 50));
        pauseSongBtn->setMaximumSize(QSize(50, 50));
        pauseSongBtn->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(pauseSongBtn);

        nextSongBtn = new QPushButton(centralwidget);
        nextSongBtn->setObjectName(QString::fromUtf8("nextSongBtn"));
        sizePolicy.setHeightForWidth(nextSongBtn->sizePolicy().hasHeightForWidth());
        nextSongBtn->setSizePolicy(sizePolicy);
        nextSongBtn->setMinimumSize(QSize(50, 50));
        nextSongBtn->setMaximumSize(QSize(50, 50));
        nextSongBtn->setIconSize(QSize(32, 32));

        horizontalLayout_2->addWidget(nextSongBtn);


        controlLayout->addLayout(horizontalLayout_2);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::MinimumExpanding, QSizePolicy::Minimum);

        controlLayout->addItem(horizontalSpacer_2);

        repeatBtn = new QPushButton(centralwidget);
        repeatBtn->setObjectName(QString::fromUtf8("repeatBtn"));
        sizePolicy.setHeightForWidth(repeatBtn->sizePolicy().hasHeightForWidth());
        repeatBtn->setSizePolicy(sizePolicy);
        repeatBtn->setMinimumSize(QSize(50, 50));
        repeatBtn->setMaximumSize(QSize(50, 50));
        repeatBtn->setIconSize(QSize(32, 32));

        controlLayout->addWidget(repeatBtn);


        verticalLayout->addLayout(controlLayout);

        progressBar = new QProgressBar(centralwidget);
        progressBar->setObjectName(QString::fromUtf8("progressBar"));
        QSizePolicy sizePolicy3(QSizePolicy::Minimum, QSizePolicy::Fixed);
        sizePolicy3.setHorizontalStretch(0);
        sizePolicy3.setVerticalStretch(0);
        sizePolicy3.setHeightForWidth(progressBar->sizePolicy().hasHeightForWidth());
        progressBar->setSizePolicy(sizePolicy3);
        progressBar->setMinimumSize(QSize(0, 0));
        progressBar->setMaximumSize(QSize(16777215, 16777215));
        progressBar->setValue(24);
        progressBar->setTextVisible(false);
        progressBar->setInvertedAppearance(false);

        verticalLayout->addWidget(progressBar);

        playlist = new QListView(centralwidget);
        playlist->setObjectName(QString::fromUtf8("playlist"));

        verticalLayout->addWidget(playlist);

        playlistControlLayout = new QHBoxLayout();
        playlistControlLayout->setSpacing(15);
        playlistControlLayout->setObjectName(QString::fromUtf8("playlistControlLayout"));
        loadPlaylistBtn = new QPushButton(centralwidget);
        loadPlaylistBtn->setObjectName(QString::fromUtf8("loadPlaylistBtn"));
        sizePolicy.setHeightForWidth(loadPlaylistBtn->sizePolicy().hasHeightForWidth());
        loadPlaylistBtn->setSizePolicy(sizePolicy);
        loadPlaylistBtn->setMinimumSize(QSize(50, 50));
        loadPlaylistBtn->setMaximumSize(QSize(50, 50));
        loadPlaylistBtn->setIconSize(QSize(32, 32));

        playlistControlLayout->addWidget(loadPlaylistBtn);

        horizontalSpacer_3 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        playlistControlLayout->addItem(horizontalSpacer_3);

        addSongBtn = new QPushButton(centralwidget);
        addSongBtn->setObjectName(QString::fromUtf8("addSongBtn"));
        addSongBtn->setMinimumSize(QSize(50, 50));
        addSongBtn->setMaximumSize(QSize(50, 50));
        addSongBtn->setIconSize(QSize(32, 32));

        playlistControlLayout->addWidget(addSongBtn);

        popSongBtn = new QPushButton(centralwidget);
        popSongBtn->setObjectName(QString::fromUtf8("popSongBtn"));
        popSongBtn->setMinimumSize(QSize(50, 50));
        popSongBtn->setMaximumSize(QSize(50, 50));
        popSongBtn->setIconSize(QSize(32, 32));

        playlistControlLayout->addWidget(popSongBtn);


        verticalLayout->addLayout(playlistControlLayout);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 600, 22));
        menuMenu = new QMenu(menubar);
        menuMenu->setObjectName(QString::fromUtf8("menuMenu"));
        MainWindow->setMenuBar(menubar);

        menubar->addAction(menuMenu->menuAction());

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        albumImage->setText(QString());
        songNameLabel->setText(QCoreApplication::translate("MainWindow", "<html><head/><body><p><span style=\" font-size:18pt; font-weight:600;\">Song Label</span></p></body></html>", nullptr));
        fileLabel->setText(QCoreApplication::translate("MainWindow", "Filename", nullptr));
        stopSongBtn->setText(QString());
        previousSongBtn->setText(QString());
        playSongBtn->setText(QString());
        pauseSongBtn->setText(QString());
        nextSongBtn->setText(QString());
        repeatBtn->setText(QString());
        loadPlaylistBtn->setText(QString());
        addSongBtn->setText(QString());
        popSongBtn->setText(QString());
        menuMenu->setTitle(QCoreApplication::translate("MainWindow", "Menu", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
