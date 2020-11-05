#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      m_ui(new Ui::MainWindow)
{
    // UI INIT
    m_ui->setupUi(this);

    m_ui->treeWidget->setColumnCount(5);
    QStringList labels;
    labels << "Title" << "Artist" << "Album"
           << "Genre" << "Path";
    m_ui->treeWidget->setHeaderLabels(labels);
    m_ui->treeWidget->setSortingEnabled(true);

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

    // PLAYER && PLAYLIST SETUP
    m_playlist = new Playlist(m_ui->treeWidget);

    // CONNECT SIGNALS
    connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::addSong);
    connect(m_ui->previousSongBtn, &QPushButton::clicked, m_playlist, &Playlist::prev);
    connect(m_ui->nextSongBtn, &QPushButton::clicked, m_playlist, &Playlist::next);
    connect(m_ui->playSongBtn, &QPushButton::clicked, m_playlist, &Playlist::play);
    connect(m_ui->pauseSongBtn, &QPushButton::clicked, m_playlist, &Playlist::pause);
    connect(m_ui->stopSongBtn, &QPushButton::clicked, m_playlist, &Playlist::stop);
    connect(m_ui->treeWidget->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](const QModelIndex &index) {
        m_playlist->setCurrent(index.row());
    });
    connect(m_playlist, &Playlist::currentSongChanged, [this](const QString& song){
//        qDebug() << m_ui->treeWidget->topLevelItem(index)->text(0);
        m_ui->songNameLabel->setText(song);
    });
}

MainWindow::~MainWindow() {
    delete m_ui;
//    delete m_player;
}

void MainWindow::addSong() {
    qDebug() << "Add song pressed";

    QStringList files = QFileDialog::getOpenFileNames(this, tr("Choose Audio File"), QDir::homePath(),
                                                      "Audio Files(*.mp3 *.wav *.mp4)");
    foreach (QString filePath, files) {
        TagLib::FileRef f(filePath.toUtf8().constData());
        TagLib::Tag* tags = f.tag();
        if (!tags->isEmpty()) {
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, tr(tags->title().toCString()));
            item->setText(1, tr(tags->artist().toCString()));
            item->setText(2, tr(tags->album().toCString()));
            item->setText(3, tr(tags->genre().toCString()));
            item->setText(4, filePath);
            m_ui->treeWidget->addTopLevelItem(item);
        } else {
            qDebug() << "Tags are empty";
            QTreeWidgetItem* item = new QTreeWidgetItem();
            item->setText(0, QFileInfo(filePath).fileName());
            item->setText(1, tr("No artist tag"));
            item->setText(2, tr("No album tag"));
            item->setText(3, tr("No genre tag"));
            item->setText(4, filePath);
            m_ui->treeWidget->addTopLevelItem(item);
        }
    }
}
