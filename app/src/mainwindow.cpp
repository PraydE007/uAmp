#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow),
      m_player(new QMediaPlayer(this)),
      m_playlist(new QMediaPlaylist(this)),
      m_playListModel(new QStandardItemModel(this))
{
    // UI INIT
    m_ui->setupUi(this);
    m_ui->playlistView->setModel(m_playListModel);
    m_playListModel->setHorizontalHeaderLabels(QStringList()  << tr("Audio Track")
                                                              << tr("File Path"));
    m_ui->playlistView->hideColumn(1);
    m_ui->playlistView->verticalHeader()->setVisible(false);
    m_ui->playlistView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_ui->playlistView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_ui->playlistView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_ui->playlistView->horizontalHeader()->setStretchLastSection(true);

    m_player->setPlaylist(m_playlist);
    m_player->setVolume(100);
    m_playlist->setPlaybackMode(QMediaPlaylist::Loop);

    // SETUP IMAGES
    m_ui->albumImage->setPixmap(QPixmap::fromImage(emptyImage));
    m_ui->previousSongBtn->setIcon(previousIcon);
    m_ui->stopSongBtn->setIcon(stopIcon);
    m_ui->playSongBtn->setIcon(playIcon);
    m_ui->pauseSongBtn->setIcon(pauseIcon);
    m_ui->nextSongBtn->setIcon(nextIcon);
    m_ui->repeatBtn->setIcon(repeatIcon);
    m_ui->loadPlaylistBtn->setIcon(listIcon);
    m_ui->addSongBtn->setIcon(plusIcon);
    m_ui->popSongBtn->setIcon(minusIcon);

    // CONNECT SIGNALS
    connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::addSong);
    connect(m_ui->previousSongBtn, &QPushButton::clicked, m_playlist, &QMediaPlaylist::previous);
    connect(m_ui->nextSongBtn, &QPushButton::clicked, m_playlist, &QMediaPlaylist::next);
    connect(m_ui->playSongBtn, &QPushButton::clicked, this, &MainWindow::playSong);
    connect(m_ui->pauseSongBtn, &QPushButton::clicked, m_player, &QMediaPlayer::pause);
    connect(m_ui->stopSongBtn, &QPushButton::clicked, m_player, &QMediaPlayer::stop);

    connect(m_ui->playlistView, &QTableView::clicked, [this](const QModelIndex &index) {
        m_playlist->setCurrentIndex(index.row());
    });

    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, [this](int index) {
        m_ui->songNameLabel->setText(m_playListModel->data(m_playListModel->index(index, 0)).toString());
    });
}

MainWindow::~MainWindow() {
    delete m_ui;
    delete m_player;
}

void MainWindow::addSong() {
    qDebug() << "Add song pressed";

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Choose Audio File"), QDir::homePath(),
                                                      "Audio Files(*.mp3 *.wav *.mp4)");
    foreach (QString filePath, files) {
        qDebug() << filePath;
        QList<QStandardItem *> items;
        items.append(new QStandardItem(QDir(filePath).dirName()));
        items.append(new QStandardItem(filePath));
        m_playListModel->appendRow(items);
        m_playlist->addMedia(QUrl(filePath));
    }
}

void MainWindow::playSong() {
    qDebug() << "play button clicked and song is loaded.";
    connect(m_player, &QMediaPlayer::mediaStatusChanged,
            this, [&](QMediaPlayer::MediaStatus status){
                if(status == QMediaPlayer::LoadedMedia) {
                    qDebug() << "Media loaded";
                    m_player->play();
                } else {
                    qDebug() << "Media was not loaded";
                }
            });
    m_player->play();

}
