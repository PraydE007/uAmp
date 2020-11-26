#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_progressBar(new ProgressBar(parent)), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

    m_ui->treeWidget->setColumnCount(6);
    QStringList labels;
    labels << "Name" << "Title" << "Artist" << "Album"
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
    connect(m_ui->actionChange_Theme, &QAction::triggered, this, &MainWindow::ChangeTheme);
    connect(m_ui->actionHelp, &QAction::triggered, this, &MainWindow::Help);
//    connect(m_ui->shuffleButton, &QPushButton::clicked, m_playlist, &Playlist::Shuffle);

    // LAMBDAS
    connect(m_ui->shuffleButton, &QPushButton::clicked, [this] {
        m_playlist->Shuffle();
        m_ui->songNameLabel->setText(tr(""));
        m_ui->fileLabel->setText(tr(""));
        m_ui->albumImage->setPixmap(QPixmap::fromImage(emptyImage));
    });
    connect(m_ui->treeWidget->selectionModel(), &QItemSelectionModel::currentRowChanged,
        [this](const QModelIndex &index) {
            if (index.row() >= 0) {
                qDebug() << "current Row Changed SetCurrent";
                qDebug() << "row:" << index.row();
                m_playlist->SetCurrent(index.row());
            }
        }
    );
    connect(m_ui->treeWidget, &QTreeWidget::customContextMenuRequested,this,&MainWindow::prepareMenu);
    connect(m_playlist, &Playlist::CurrentSongChanged,
        [this](const QString& song, const QString& name) {
            m_ui->songNameLabel->setText(song);
            m_ui->fileLabel->setText(name);
        }
    );
    connect(m_playlist, &Playlist::CurrentImageChanged,
        [this](QPixmap pixmap) {
            qDebug() << "Image changed!";
            m_ui->albumImage->setPixmap(pixmap);
        }
    );
    connect(m_playlist, &Playlist::NoImage,
        [this] {
            m_ui->albumImage->setPixmap(QPixmap::fromImage(emptyImage));
        }
    );

    // LOAD STATE
    ReadState1();
}

MainWindow::~MainWindow() {
    SaveState();
    delete m_ui;
    // delete m_player;
}

void MainWindow::SaveState() {
    SaveJPLAYLST("./app/res/tmp/playlist");
    SaveWindow1("./app/res/tmp/window");
}

void MainWindow::SaveWindow1(std::string fileName) {
    std::ofstream of(fileName, std::ofstream::out);

    if (!of.is_open()) {
        QMessageBox::information(this, tr("Unable to open file"), strerror(errno));
        return;
    }

    rapidjson::Document document;
    document.SetObject();

    rapidjson::Value playlists(rapidjson::kArrayType);
    for (size_t i = 0; i < m_recentPlaylists.size(); i++) {
        playlists.PushBack(
            rapidjson::Value().SetString(m_recentPlaylists[i].c_str(), m_recentPlaylists[i].size(),
                document.GetAllocator()),
            document.GetAllocator()
        );
    }
    rapidjson::Value songs(rapidjson::kArrayType);
    for (size_t i = 0; i < m_recentSongs.size(); i++) {
        songs.PushBack(
            rapidjson::Value().SetString(m_recentSongs[i].c_str(), m_recentSongs[i].size(), document.GetAllocator()),
            document.GetAllocator()
        );
    }

    SaveWindow2(document);
    document.AddMember("songs", songs, document.GetAllocator());
    document.AddMember("playlists", playlists, document.GetAllocator());

    // JSON WRITER
    rapidjson::StringBuffer buffer;
    buffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(buffer);
    document.Accept(writer);
    // WRITING
    of << buffer.GetString();
    of.close();
}

void MainWindow::SaveWindow2(rapidjson::Document& document) {
        document.AddMember("type", rapidjson::Value().SetString("SaveState"), document.GetAllocator());
    document.AddMember("mode",
        rapidjson::Value().SetInt(static_cast<int>(m_playlist->GetMode())), document.GetAllocator());
    document.AddMember("volume",
        rapidjson::Value().SetInt(m_ui->volumeSlider->value()), document.GetAllocator());
    document.AddMember(
        "currentSong",
        rapidjson::Value().SetString(m_playlist->GetUrl(), ::strlen(m_playlist->GetUrl()), document.GetAllocator()),
        document.GetAllocator()
    );
    document.AddMember("currentTime", rapidjson::Value().SetInt64(m_playlist->GetPosition()), document.GetAllocator());
    document.AddMember("currentSelected",
        rapidjson::Value().SetInt(m_ui->treeWidget->currentIndex().row()), document.GetAllocator());
}

void MainWindow::ReadState1() {
    ParseJPLAYLST("./app/res/tmp/playlist");

    rapidjson::Document document;
    std::ifstream fstream("./app/res/tmp/window");
    std::string buffer;
    std::string json;

    if (!fstream.is_open()) {
        ShowErrorOk("Configuration file was not found!");
        return;
    }
    while (std::getline(fstream, buffer))
        json += buffer + '\n';
    fstream.close();
    document.Parse(json.c_str());

    if (document.IsObject()) {
        if (document.HasMember("type") && document.HasMember("mode")
            && document.HasMember("volume") && document.HasMember("currentSong")
            && document.HasMember("currentTime") && document.HasMember("currentSelected")
            && document.HasMember("songs") && document.HasMember("playlists"))
        {
            if (document["type"].IsString() && document["mode"].IsInt()
                && document["volume"].IsInt() && document["currentSong"].IsString()
                && document["currentTime"].IsInt64() && document["currentSelected"].IsInt()
                && document["songs"].IsArray() && document["playlists"].IsArray())
            {
                if (std::string(document["type"].GetString()) == "SaveState") {
                    ReadSongsArray(document["songs"]);
                    ReadPlaylistsArray(document["playlists"]);
                    ReadState2(document);
                    return;
                }
            }
        }
    }
    ShowErrorOk("Configuration file was corrapted!");
}

void MainWindow::ReadState2(rapidjson::Document& document) {
    for (int i = 0; i < document["mode"].GetInt(); i++)
        ChangeRepeatMode();
    m_ui->volumeSlider->setValue(document["volume"].GetInt());
    m_playlist->AcceptSongByUrl(document["currentSong"].GetString());
    if (document["currentSelected"].GetInt() >= 0)
        m_playlist->SelectIndex(document["currentSelected"].GetInt());
    m_playlist->SetTime(document["currentTime"].GetInt64());
}

void MainWindow::ChangeTheme() {
    qDebug() << "Change theme please!";

    if (!m_isDark) {
        QFile File("app/res/themes/darkTheme.qss");
        File.open(QFile::ReadOnly);
        QString StyleSheet = QLatin1String(File.readAll());

        this->setStyleSheet(StyleSheet);
        m_isDark = true;
    } else {
        this->setStyleSheet("");
        m_isDark = false;
    }
}

void MainWindow::Help() {
    qDebug() << "Someone needs your help!";
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
    item->setText(0, tr(FilepathToTitle(filepath).c_str()));
    if (!tags->isEmpty()) {
        item->setText(1, tr(tags->title().toCString()));
        item->setText(2, tr(tags->artist().toCString()));
        item->setText(3, tr(tags->album().toCString()));
        item->setText(4, tr(tags->genre().toCString()));
    } else {
        item->setText(1, QFileInfo(filepath.c_str()).fileName());
        item->setText(2, tr("No artist tag"));
        item->setText(3, tr("No album tag"));
        item->setText(4, tr("No genre tag"));
    }
    item->setText(5, filepath.c_str());
    m_ui->treeWidget->addTopLevelItem(item);
    if (std::find(m_recentSongs.begin(), m_recentSongs.end(), filepath) == m_recentSongs.end())
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
    if (std::find(m_recentPlaylists.begin(), m_recentPlaylists.end(), filepath) == m_recentPlaylists.end())
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
                if (std::string(document["type"].GetString()) == "JPLAYLST") {
                    if (document.HasMember("playlist")) {
                        if (document["playlist"].IsArray()) {
                            if (std::find(m_recentPlaylists.begin(), m_recentPlaylists.end(), filepath)
                                == m_recentPlaylists.end())
                            {
                                m_recentPlaylists.push_back(filepath);
                            }
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

void MainWindow::ReadSongsArray(const rapidjson::Value& array) {
    bool ferr = false;

    for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
        if (array[i].IsString()) {
            int fd = open(array[i].GetString(), O_RDONLY);

            if (fd > 0) {
                ::close(fd);
                if (std::find(m_recentSongs.begin(), m_recentSongs.end(), array[i].GetString())
                    == m_recentSongs.end())
                {
                    m_recentSongs.push_back(array[i].GetString());
                }
            }
            else
                ferr = true;
        }
    }
    if (ferr)
        ShowMessageOk("Some songs are corrupted or missing!");
}

void MainWindow::ReadPlaylistsArray(const rapidjson::Value& array) {
    bool ferr = false;

    for (rapidjson::SizeType i = 0; i < array.Size(); i++) {
        if (array[i].IsString()) {
            int fd = open(array[i].GetString(), O_RDONLY);

            if (fd > 0) {
                ::close(fd);
                if (std::find(m_recentPlaylists.begin(), m_recentPlaylists.end(), array[i].GetString())
                    == m_recentPlaylists.end())
                {
                    m_recentPlaylists.push_back(array[i].GetString());
                }
            }
            else
                ferr = true;
        }
    }
    if (ferr)
        ShowMessageOk("Some playlists are corrupted or missing!");
}

bool MainWindow::HasDuplicate(QString fileName) {
    for (int i = 0; i < m_ui->treeWidget->topLevelItemCount(); i++) {
        if (m_ui->treeWidget->topLevelItem(i)->text(5) == fileName)
            return true;
    }
    return false;
}

void MainWindow::SavePlaylist() {
    if (m_ui->treeWidget->topLevelItemCount() > 0) {
        QString selectedFilter;
        QString fileName = QFileDialog::getSaveFileName(this,
            tr("Save Playlist"), "",
            tr("M3U Playlist (*.m3u);;M3U Unicode Playlist (*.m3u8);;JPLAYLST Playlist (*.jplaylst);;All Files (*)"),
            &selectedFilter
        );
        if (fileName.isEmpty())
            return;
        if (selectedFilter == "M3U Playlist (*.m3u)" ||
            selectedFilter == "M3U Unicode Playlist (*.m3u8)")
            SaveM3U(fileName);
        else if (selectedFilter == "JPLAYLST Playlist (*.jplaylst)")
            SaveJPLAYLST(fileName);
    }
    else {
        this->ShowMessageOk("You cannot save an empty playlist!");
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
        of << "#EXTINF:" << -1 << ',' << m_ui->treeWidget->topLevelItem(i)->text(1).toUtf8().constData() // set track duration instead of -1!!!
            << " - " << m_ui->treeWidget->topLevelItem(i)->text(0).toUtf8().constData() << '\n';
        of << m_ui->treeWidget->topLevelItem(i)->text(5).toUtf8().constData() << '\n';
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
        const char* string = m_ui->treeWidget->topLevelItem(i)->text(5).toUtf8().constData();

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
                                                 nd->text(1), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(5).toUtf8().data());

                f.tag()->setTitle(text.toUtf8().data());
                nd->setText(1, text);
                m_ui->songNameLabel->setText(text);
                f.save();

            }
        });
        connect(setArtist, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Artist"),
                                                 tr("Artist:"), QLineEdit::Normal,
                                                 nd->text(2), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(5).toUtf8().data());

                f.tag()->setArtist(text.toUtf8().data());
                nd->setText(1, text);
                f.save();
            }
        });
        connect(setAlbum, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Album"),
                                                 tr("Album:"), QLineEdit::Normal,
                                                 nd->text(3), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(5).toUtf8().data());

                f.tag()->setAlbum(text.toUtf8().data());
                nd->setText(3, text);
                f.save();
            }
        });
        connect(setGenre, &QAction::triggered, [this, nd]() {
            bool ok;

            QString text = QInputDialog::getText(this, tr("New Genre"),
                                                 tr("Genre:"), QLineEdit::Normal,
                                                 nd->text(3), &ok);
            if (ok && !text.isEmpty()) {
                TagLib::FileRef f(nd->text(5).toUtf8().data());

                f.tag()->setGenre(text.toUtf8().data());
                nd->setText(4, text);
                f.save();
            }
        });

        connect(setImage, &QAction::triggered, [this, nd] {
            std::string fileName = nd->text(5).toUtf8().data();
            if (fileName.substr(fileName.size() - 3) == "mp3") {
                QString image = QFileDialog::getOpenFileName(
                        this,
                        tr("Choose New Image"),
                        QDir::homePath(),
                        "Image(*.jpg)"
                );
                if (!image.isEmpty()) {
                    setNewImage(nd->text(5).toUtf8().data(), image.toUtf8().data());
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

void MainWindow::createDialog(const std::vector<std::string>& m_recentFiles) {
    QDialog dialog(this);
    dialog.setModal(true);
    dialog.setWindowTitle("Recently opened");

    // TABLE WIDGET
    QTableWidget *table = new QTableWidget(m_recentFiles.size(), 2, this);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->verticalHeader()->setVisible(false);
    table->horizontalHeader()->setVisible(false);

    // SIZE POLICY
    QHeaderView* header = table->horizontalHeader();
    header->setSectionResizeMode(QHeaderView::Stretch);

    // HORIZONTAL LAYOUT
    QHBoxLayout *hTableLayout = new QHBoxLayout;
    hTableLayout->addWidget(table);

    // VERTICAL LAYOUT
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addLayout(hTableLayout);

    // FILL TABLE
    for (auto i = 0ul; i < m_recentFiles.size(); ++i) {
        qDebug() << m_recentFiles[i].c_str();
        QFileInfo file(m_recentFiles[i].c_str());
        qDebug() << file.baseName();
        qDebug() << file.filePath();
        table->setItem(i, 0, new QTableWidgetItem(file.baseName()));
        table->setItem(i, 1, new QTableWidgetItem(file.filePath()));
    }
    table->hideColumn(1);

    // CONNECTIONS
    connect(table, &QTableWidget::itemDoubleClicked, [this, table] (QTableWidgetItem *item) {
        int row = item->row();
        QTableWidgetItem* itemPath = table->item(row, 1);
        std::string path = itemPath->text().toUtf8().data();

        if ((path.substr(path.size() - 4) == ".mp3"
            || path.substr(path.size() - 4) == ".wav")
            && !HasDuplicate(QString(path.c_str()))) {

            // ADD TO PLAYLIST MESSAGE BOX
            int resp = QMessageBox::question(this, tr("Select"),
                                             tr("New item was chosen to open.\n"
                                                "Do you want to add it to opened playlist?"),
                                             QMessageBox::Ok | QMessageBox::Cancel);
            if (resp == QMessageBox::Ok) {
                    LoadSong(path);
            } else {
                m_playlist->UnselectList();
                m_playlist->ClearPlaylist();
                LoadSong(path);
            }
        } else {
            if ((path.size() > 4 && path.substr(path.size() - 4, 4) == ".m3u")
                || (path.size() > 5 && path.substr(path.size() - 5, 5) == ".m3u8"))
            {
                m_playlist->UnselectList();
                m_playlist->ClearPlaylist();
                ParseM3U(path);
            } else if (path.size() > 9 && path.substr(path.size() - 9, 9) == ".jplaylst") {
                m_playlist->UnselectList();
                m_playlist->ClearPlaylist();
                ParseJPLAYLST(path);
            }
        }
    });
    dialog.setLayout(vLayout);
    dialog.exec();
}

void MainWindow::openRecentPlaylists() {
    qDebug() << "open recent playlists";
    if (m_recentPlaylists.empty()) {
        this->ShowMessageOk("There is no recently opened playlists.");
    } else {
        createDialog(m_recentPlaylists);
    }
}

void MainWindow::openRecentSongs() {
    qDebug() << "open recent songs";
    if (m_recentSongs.empty()) {
        this->ShowMessageOk("There is no recently opened songs");
    } else {
        createDialog(m_recentSongs);
    }
}
