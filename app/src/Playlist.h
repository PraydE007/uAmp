#pragma once

//STANDARD LIB
#include <vector>
#include <algorithm>
#include <iterator>

//QT
#include "q_includes.h"

class Playlist : public QObject {
    Q_OBJECT

public:
    Playlist(QTreeWidget* treeWidget);

    //SONG MANAGEMENT
    void play();
    void pause();
    void stop();

    //PLAYLIST MANAGEMENT
    void del(const QString& filePath);
    void next();
    void prev();
    void setCurrent(int index);

    //TREE WIDGET MANAGEMENT
    QWidget* getTreeWidget();

public slots:


signals:
    void currentSongChanged(QString song);

private:
    int m_currentRow = 0;
    QTreeWidget* m_treeWidget;
    QMediaPlayer* m_player;
};
