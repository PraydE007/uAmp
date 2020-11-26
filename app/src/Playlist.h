#pragma once

// STANDARD LIB
#include <vector>
#include <algorithm>
#include <iterator>

// QT
#include "q_includes.h"

class ProgressBar;

class Playlist : public QObject {
    Q_OBJECT

public:
    enum RepeatMode {
        NoRepeat,
        RepeatSong,
        RepeatPlaylist
    };

    Playlist(QTreeWidget* treeWidget, ProgressBar *progressBar, QSlider* slider);

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
    void Rewind();
    void Forward();
    void SetCurrent(int index);
    void Shuffle();

    RepeatMode GetMode();

    // INTERFACE
    void AcceptSong(QString filepath);

    // TREE WIDGET MANAGEMENT
    QWidget* GetTreeWidget();

signals:
    void LoopSong();
    void LoopPlaylist();
    void NoLoop();
    void CurrentSongChanged(QString title, QString name);
    void CurrentImageChanged(QPixmap pixMap);
    void NoImage();

private:
    // COVER SETUP
    QByteArray getCover(const QString& songPath);

    QSlider* m_slider;
    ProgressBar* m_progressBar;
    RepeatMode m_mode{NoRepeat};
    QTreeWidget* m_treeWidget;
    QMediaPlayer* m_player;
    QMediaPlaylist* m_playlist;

    // TO TRACK END OF SONG
    qint64 m_pos{0};
};
