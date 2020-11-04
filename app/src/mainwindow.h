#ifndef MAINWINDOW_H
#define MAINWINDOW_H

// LOCAL
#include "Playlist.h"

// STL
#include <vector>

// QT
//#include "q_includes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void addSong();

private:
    // ICONS
    QImage emptyImage{"app/res/images/empty-image.png"};
    QIcon previousIcon{"app/res/images/previous.png"};
    QIcon stopIcon{"app/res/images/stop.png"};
    QIcon playIcon{"app/res/images/play.png"};
    QIcon pauseIcon{"app/res/images/pause.png"};
    QIcon nextIcon{"app/res/images/next.png"};
    QIcon repeatIcon{"app/res/images/repeat.png"};
    QIcon listIcon{"app/res/images/list.png"};
    QIcon plusIcon{"app/res/images/plus.png"};
    QIcon minusIcon{"app/res/images/minus.png"};

    // AUDIO FILES
    Playlist* m_playlist;
//    std::vector<AudioFile*> m_songsVector;

    // QT OBJECTS
    Ui::MainWindow *m_ui;
//    QMediaPlayer* m_player;
//    QMediaPlaylist* m_playlist;
    QStandardItemModel* m_playListModel;
};

#endif // MAINWINDOW_H
