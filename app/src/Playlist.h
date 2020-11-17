#pragma once

// STANDARD LIB
#include <vector>
#include <algorithm>
#include <iterator>

// QT
#include "q_includes.h"

class Playlist : public QObject {
    Q_OBJECT

public:
    enum RepeatMode {
        NoRepeat,
        RepeatSong,
        RepeatPlaylist
    };

    Playlist(QTreeWidget* treeWidget);

    // SONG MANAGEMENT
    void Play();
    void Pause();
    void Stop();

    // PLAYLIST MANAGEMENT
    void ChangeRepeatMode();
    void ClearPlaylist();
    void UnselectList();
    void PopSong();
    void Next();
    void Prev();
    void SetCurrent(int index);
    void Shuffle();

    RepeatMode GetMode();

    // INTERFACE
    void AcceptSong(QString filepath);

    // TREE WIDGET MANAGEMENT
    QWidget* GetTreeWidget();

signals:
    void CurrentSongChanged(QString song);

private:
    RepeatMode m_mode{NoRepeat};
    QTreeWidget* m_treeWidget;
    QMediaPlayer* m_player;
};
