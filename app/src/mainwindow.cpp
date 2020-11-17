#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_progressBar(new ProgressBar(parent)), m_ui(new Ui::MainWindow) {
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
    m_ui->shuffleButton->setIcon(shuffleIcon);

    // PROGRESS BAR REPLACEMENT
    m_ui->progressBar->parentWidget()->layout()->replaceWidget(m_ui->progressBar, m_progressBar);
    delete m_ui->progressBar;

    // PLAYER && PLAYLIST SETUP
    m_playlist = new Playlist(m_ui->treeWidget, m_progressBar, m_ui->volumeSlider);

    // ABONDARENK CONNECTS
    connect(m_ui->loadPlaylistBtn, &QPushButton::clicked, this, &MainWindow::OpenPlaylist);
    connect(m_ui->savePlaylistBtn, &QPushButton::clicked, this, &MainWindow::SavePlaylist);
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
    connect(m_ui->shuffleButton, &QPushButton::clicked, m_playlist, &Playlist::Shuffle);

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

    foreach (QString filePath, files) {
        if (!HasDuplicate(filePath))
            LoadSong(filePath.toUtf8().constData());
    }
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

    dialog.setNameFilter(tr("M3U Playlist (*.m3u *.m3u8);;JPLAYLST Playlist (*.jplaylst)"));
    if (dialog.exec()) {
        filepath = dialog.selectedFiles()[0];

        std::string fp(filepath.toUtf8().constData());
        if ((fp.size() > 4 && fp.substr(fp.size() - 4, 4) == ".m3u")
            || (fp.size() > 5 && fp.substr(fp.size() - 5, 5) == ".m3u8"))
        {
            m_playlist->UnselectList();
            m_playlist->ClearPlaylist();
            ParseM3U(fp);
        } else if (fp.size() > 9 && fp.substr(fp.size() - 9, 9) == ".jplaylst") {
            m_playlist->UnselectList();
            m_playlist->ClearPlaylist();
            ParseJPLAYLST(fp);
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
            if (!HasDuplicate(filepath.c_str()))
                LoadSong(buffer);
        }
    }
    m_recentPlaylists.push_back(filepath);
    fstream.close();
}

void MainWindow::ParseJPLAYLST(std::string filepath) {
    rapidjson::Document document;
    std::ifstream fstream(filepath);
    std::string buffer;
    std::string json;

    if (!fstream.is_open()) {
        ShowErrorOk("The file is corrupted or unreadable!");
        return;
    }
    while (std::getline(fstream, buffer))
        json += buffer + '\n';
    fstream.close();
    document.Parse(json.c_str());
    if (document.IsObject()) {
        if (document.HasMember("type")) {
            if (document["type"].IsString()) {
                if ((std::string(document["type"].GetString()) == "JPLAYLST")) {
                    if (document.HasMember("playlist")) {
                        if (document["playlist"].IsArray()) {
                            m_recentPlaylists.push_back(filepath);
                            JPLAYLST_ArrayRead(document["playlist"]);
                            return;
                        }
                    }
                }
            }
        }
    }
    ShowErrorOk("The file does not meet JPLAYLST standard!");
}

void MainWindow::JPLAYLST_ArrayRead(const rapidjson::Value& array) {
    bool ferr = false;

    for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
        if (array[i].IsString()) {
            int fd = open(array[i].GetString(), O_RDONLY);

            if (fd > 0) {
                ::close(fd);
                if (!HasDuplicate(array[i].GetString()))
                    LoadSong(array[i].GetString());
            }
            else
                ferr = true;
        }
    }
    if (ferr)
        ShowMessageOk("Some files are corrupted or missing!");
}

bool MainWindow::HasDuplicate(QString fileName) {
    for (int i = 0; i < m_ui->treeWidget->topLevelItemCount(); i++) {
        if (m_ui->treeWidget->topLevelItem(i)->text(4) == fileName)
            return true;
    }
    return false;
}

void MainWindow::SavePlaylist() {
    QString selectedFilter;
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save Playlist"), "",
        tr("M3U Playlist (*.m3u);;M3U Unicode Playlist (*.m3u8);;JPLAYLST Playlist (*.jplaylst);;All Files (*)"),
        &selectedFilter
    );

    if (fileName.isEmpty())
        return;
    else if (m_ui->treeWidget->topLevelItemCount() > 0) {
        if (selectedFilter == "M3U Playlist (*.m3u)" ||
            selectedFilter == "M3U Unicode Playlist (*.m3u8)")
            SaveM3U(fileName);
        else if (selectedFilter == "JPLAYLST Playlist (*.jplaylst)")
            SaveJPLAYLST(fileName);
    }
}

void MainWindow::SaveM3U(QString fileName) {
    std::ofstream of(fileName.toUtf8().constData(), std::ofstream::out);

    if (!of.is_open()) {
        QMessageBox::information(this, tr("Unable to open file"), strerror(errno));
        return;
    }

    of << "#EXTM3U" << '\n';
    for (int i = 0; i < m_ui->treeWidget->topLevelItemCount(); i++) {
        of << '\n';
        of << "#EXTINF:" << -1 << ',' << m_ui->treeWidget->topLevelItem(i)->text(1).toUtf8().constData()
            << " - " << m_ui->treeWidget->topLevelItem(i)->text(0).toUtf8().constData() << '\n';
        of << m_ui->treeWidget->topLevelItem(i)->text(4).toUtf8().constData() << '\n';
    }
    of.close();
}

void MainWindow::SaveJPLAYLST(QString fileName) {
    std::ofstream of(fileName.toUtf8().constData(), std::ofstream::out);

    if (!of.is_open()) {
        QMessageBox::information(this, tr("Unable to open file"), strerror(errno));
        return;
    }

    rapidjson::Document document;
    document.SetObject();

    rapidjson::Value playlist(rapidjson::kArrayType);
    for (int i = 0; i < m_ui->treeWidget->topLevelItemCount(); i++) {
        const char* string = m_ui->treeWidget->topLevelItem(i)->text(4).toUtf8().constData();

        playlist.PushBack(
            rapidjson::Value().SetString(string, ::strlen(string), document.GetAllocator()),
            document.GetAllocator()
        );
    }

    document.AddMember("type", rapidjson::Value().SetString("JPLAYLST"), document.GetAllocator());
    document.AddMember("playlist", playlist, document.GetAllocator());

    // JSON WRITER
    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);

    // WRITING
    of << buffer.GetString();
    of.close();
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
