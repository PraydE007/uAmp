#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
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
    m_ui->loadPlaylistBtn->setIcon(playlistIcon);
    m_ui->savePlaylistBtn->setIcon(saveIcon);
    m_ui->addSongBtn->setIcon(plusIcon);
    m_ui->popSongBtn->setIcon(minusIcon);

    // PLAYER && PLAYLIST SETUP
    m_playlist = new Playlist(m_ui->treeWidget);

    // ABONDARENK CONNECTS
    connect(m_ui->loadPlaylistBtn, &QPushButton::clicked, this, &MainWindow::OpenPlaylist);
    // connect(m_ui->repeatBtn, &QPushButton::clicked, this, &MainWindow::ChangeRepeatMode);

    // WARN
    // connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::NextSong);

    // CONNECT SIGNALS
    connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::OpenSong);
    connect(m_ui->previousSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Prev);
    connect(m_ui->nextSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Next);
    connect(m_ui->playSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Play);
    connect(m_ui->pauseSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Pause);
    connect(m_ui->stopSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Stop);

    // LAMBDAS
    connect(m_ui->treeWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
        [this](const QModelIndex &index) {
            m_playlist->SetCurrent(index.row());
        }
    );
    connect(m_playlist, &Playlist::CurrentSongChanged,
        [this](const QString& song) {
            m_ui->songNameLabel->setText(song);
        }
    );
}

MainWindow::~MainWindow() {
    delete m_ui;
    // delete m_player;
}

void MainWindow::OpenSong() {
    QStringList files = QFileDialog::getOpenFileNames(
        this,
        tr("Choose Audio File"),
        QDir::homePath(),
        "Audio Files(*.mp3 *.wav *.mp4)"
    );

    foreach (QString filePath, files)
        LoadSong(filePath.toUtf8().constData());
}

void MainWindow::LoadSong(std::string filepath) {
    TagLib::FileRef f(filepath.c_str());
    TagLib::Tag* tags = f.tag();

    if (!tags->isEmpty()) {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, tr(tags->title().toCString()));
        item->setText(1, tr(tags->artist().toCString()));
        item->setText(2, tr(tags->album().toCString()));
        item->setText(3, tr(tags->genre().toCString()));
        item->setText(4, filepath.c_str());
        m_ui->treeWidget->addTopLevelItem(item);
    } else {
        QTreeWidgetItem* item = new QTreeWidgetItem();
        item->setText(0, QFileInfo(filepath.c_str()).fileName());
        item->setText(1, tr("No artist tag"));
        item->setText(2, tr("No album tag"));
        item->setText(3, tr("No genre tag"));
        item->setText(4, filepath.c_str());
        m_ui->treeWidget->addTopLevelItem(item);
    }
}

void MainWindow::ChangeRepeatMode() {
    if (repeatMode == NoRepeat) {
        repeatMode = RepeatSong;
    } else if (repeatMode == RepeatSong) {
        repeatMode = RepeatPlaylist;
    } else {
        repeatMode = NoRepeat;
    }
}

void MainWindow::OpenPlaylist() {
    QFileDialog dialog(this);
    QString filepath;

    dialog.setNameFilter(tr("Playlist (*.m3u *.m3u8)"));
    if (dialog.exec()) {
        filepath = dialog.selectedFiles()[0];

        std::string fp(filepath.toUtf8().constData());
        if (fp.size() < 5 || fp.substr(fp.size() - 4, 4) != ".m3u"
            || fp.size() < 6 || fp.substr(fp.size() - 5, 5) != ".m3u8")
        {
            m_playlist->ClearPlaylist();
            ParseM3U(fp);
        }
    }
}

void MainWindow::ParseM3U(std::string filepath) {
    std::ifstream fstream(filepath);
    std::string buffer;

    if (!fstream.is_open()) {
        ShowErrorOk("The file is corrupted or unreadable!");
        return;
    }
    while (std::getline(fstream, buffer, '\n')) {
        int fd = open(buffer.c_str(), O_RDONLY);

        if (fd > 0) {
            ::close(fd);
            LoadSong(buffer);
        }
    }
    fstream.close();
}

void MainWindow::ParseJPLAYLST(std::string filepath) {
    
}

void MainWindow::SavePlaylist() {

}

void MainWindow::ShowMessageOk(std::string message) {
    QMessageBox::information(
        this,
        tr("Message"),
        tr(message.c_str()),
        QMessageBox::Ok,
        QMessageBox::Ok
    );
}

void MainWindow::ShowErrorOk(std::string message) {
    QMessageBox::critical(
        this,
        tr("Message"),
        tr(message.c_str()),
        QMessageBox::Ok,
        QMessageBox::Ok
    );
}
