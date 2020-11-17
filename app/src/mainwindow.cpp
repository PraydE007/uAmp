#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_progressBar(new ProgressBar(parent)),
    m_ui(new Ui::MainWindow)
    {
    m_ui->setupUi(this);

    m_ui->treeWidget->setColumnCount(5);
    QStringList labels;
    labels << "Title" << "Artist" << "Album"
           << "Genre" << "Path";
    m_ui->treeWidget->setHeaderLabels(labels);
    m_ui->treeWidget->setSortingEnabled(true);
    m_ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);

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
    m_ui->fastForwardBtn->setIcon(forwardIcon);
    m_ui->rewindBtn->setIcon(rewindIcon);

    // PROGRESS BAR REPLACEMENT
    m_ui->progressBar->parentWidget()->layout()->replaceWidget(m_ui->progressBar, m_progressBar);
    delete m_ui->progressBar;

    // PLAYER && PLAYLIST SETUP
    m_playlist = new Playlist(m_ui->treeWidget, m_progressBar, m_ui->volumeSlider);

    // ABONDARENK CONNECTS
    connect(m_ui->loadPlaylistBtn, &QPushButton::clicked, this, &MainWindow::OpenPlaylist);
    connect(m_ui->repeatBtn, &QPushButton::clicked, this, &MainWindow::ChangeRepeatMode);

    // CONNECT SIGNALS
    connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::OpenSong);
    connect(m_ui->previousSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Prev);
    connect(m_ui->nextSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Next);
    connect(m_ui->playSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Play);
    connect(m_ui->pauseSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Pause);
    connect(m_ui->stopSongBtn, &QPushButton::clicked, m_playlist, &Playlist::Stop);
    connect(m_ui->popSongBtn, &QPushButton::clicked, m_playlist, &Playlist::PopSong);
    connect(m_ui->rewindBtn, &QPushButton::clicked, m_playlist, &Playlist::Rewind);
    connect(m_ui->fastForwardBtn, &QPushButton::clicked, m_playlist, &Playlist::Forward);
    connect(m_ui->actionShow_Playlists, &QAction::triggered, this, &MainWindow::openRecentPlaylists);
    connect(m_ui->actionShow_Songs, &QAction::triggered, this, &MainWindow::openRecentSongs);

    // LAMBDAS
    connect(m_ui->treeWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
        [this](const QModelIndex &index) {
            if (index.row() >= 0)
                m_playlist->SetCurrent(index.row());
        }
    );
    connect(m_ui->treeWidget, &QTreeWidget::customContextMenuRequested,this,&MainWindow::prepareMenu);
    connect(m_playlist, &Playlist::CurrentSongChanged,
        [this](const QString& song) {
            m_ui->songNameLabel->setText(song);
        }
    );
    connect(m_playlist, &Playlist::CurrentImageChanged,
        [this](QPixmap pixmap) {
            m_ui->albumImage->setPixmap(pixmap);
        }
    );
    connect(m_playlist, &Playlist::NoImage,
        [this] {
            m_ui->albumImage->setPixmap(QPixmap::fromImage(emptyImage));
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

    QTreeWidgetItem* item = new QTreeWidgetItem();
    if (!tags->isEmpty()) {
        if (tags->title().isEmpty())
            item->setText(0, tr(FilepathToTitle(filepath).c_str()));
        else
            item->setText(0, tr(tags->title().toCString()));
        item->setText(1, tr(tags->artist().toCString()));
        item->setText(2, tr(tags->album().toCString()));
        item->setText(3, tr(tags->genre().toCString()));
    } else {
        item->setText(0, QFileInfo(filepath.c_str()).fileName());
        item->setText(1, tr("No artist tag"));
        item->setText(2, tr("No album tag"));
        item->setText(3, tr("No genre tag"));
    }
    item->setText(4, filepath.c_str());
    m_ui->treeWidget->addTopLevelItem(item);
    m_recentSongs.push_back(filepath);
}

std::string MainWindow::FilepathToTitle(std::string filepath) {
    int slash = filepath.size();
    for (; filepath[slash] != '/' && slash > 0; slash--);

    std::string file(filepath.begin() + slash + 1, filepath.end());

    int dot = file.size();
    for (; file[dot] != '.' && dot > 0; dot--);

    std::string title(file.begin(), file.begin() + dot);
    return title;
}

void MainWindow::ChangeRepeatMode() {
    if (m_playlist->GetMode() == Playlist::NoRepeat)
        m_ui->repeatModeText->setText("Repeat Song");
    else if (m_playlist->GetMode() == Playlist::RepeatSong)
        m_ui->repeatModeText->setText("Repeat Playlist");
    else
        m_ui->repeatModeText->setText("No Repeat");
    m_playlist->ChangeRepeatMode();
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
            m_playlist->UnselectList();
            m_playlist->ClearPlaylist();
            ParseM3U(fp);
            m_recentPlaylists.push_back(fp);
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

// CUSTOM CONTEXT MENU
void MainWindow::prepareMenu(const QPoint & pos) {
    QTreeWidget *tree = m_ui->treeWidget;

    QTreeWidgetItem *nd = tree->itemAt(pos);

    if (nd) {
        qDebug() << pos << nd->text(0);

        QAction *setTittle = new QAction(tr("Edit title"), this);
        QAction *setArtist = new QAction(tr("Edit artist"), this);
        QAction *setAlbum = new QAction(tr("Edit album"), this);
        QAction *setGenre = new QAction(tr("Edit genre"), this);
        QAction *setImage = new QAction(tr("Edit image"), this);

        connect(setTittle, &QAction::triggered, [this, nd] () {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Title"),
                                                 tr("Title:"), QLineEdit::Normal,
                                                 nd->text(0), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(4).toUtf8().data());

                f.tag()->setTitle(text.toUtf8().data());
                nd->setText(0, text);
                m_ui->songNameLabel->setText(text);
                f.save();

            }
        });
        connect(setArtist, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Artist"),
                                                 tr("Artist:"), QLineEdit::Normal,
                                                 nd->text(1), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(4).toUtf8().data());

                f.tag()->setArtist(text.toUtf8().data());
                nd->setText(1, text);
                f.save();
            }
        });
        connect(setAlbum, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Album"),
                                                 tr("Album:"), QLineEdit::Normal,
                                                 nd->text(2), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(4).toUtf8().data());

                f.tag()->setAlbum(text.toUtf8().data());
                nd->setText(2, text);
                f.save();
            }
        });
        connect(setGenre, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Genre"),
                                                 tr("Genre:"), QLineEdit::Normal,
                                                 nd->text(3), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(4).toUtf8().data());

                f.tag()->setGenre(text.toUtf8().data());
                nd->setText(3, text);
                f.save();
            }
        });

        connect(setImage, &QAction::triggered, [this, nd] {
            std::string fileName = nd->text(4).toUtf8().data();
            if (fileName.substr(fileName.size() - 3) == "mp3") {
                QString image = QFileDialog::getOpenFileName(
                        this,
                        tr("Choose New Image"),
                        QDir::homePath(),
                        "Image(*.jpg)"
                );
                if (!image.isEmpty()) {
                    setNewImage(nd->text(4).toUtf8().data(), image.toUtf8().data());
                m_ui->albumImage->setPixmap(QPixmap::fromImage(QImage(image)));
                }
            } else {
                this->ShowMessageOk("*.wav formats are not supported.");
            }
        });

      QMenu menu(this);
      menu.addAction(setTittle);
      menu.addAction(setArtist);
      menu.addAction(setAlbum);
      menu.addAction(setGenre);
      menu.addAction(setImage);
      menu.exec(tree->mapToGlobal(pos));
    }
}

void MainWindow::openRecentPlaylists() {
    qDebug() << "open recent playlists";
    if (m_recentPlaylists.empty()) {
        this->ShowMessageOk("There is no recently opened playlists.");
    } else {

    }
}

void MainWindow::openRecentSongs() {
    qDebug() << "open recent songs";
    if (m_recentSongs.empty()) {
        this->ShowMessageOk("There is no recently opened songs");
    } else {

    }
}
