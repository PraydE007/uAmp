#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "q_includes.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
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

    Ui::MainWindow *m_ui;
};

#endif // MAINWINDOW_H
