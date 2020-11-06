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

    // PLAYER && PLAYLIST SETUP
    m_playlist = new Playlist(m_ui->treeWidget);

    // PROGRESSBAR SETUP
    m_ui->progressBar->setRange(0, 100);
    m_ui->progressBar->setValue(0);
    m_ui->progressBar->setEnabled(true);

    // ABONDARENK CONNECTS
    connect(m_ui->loadPlaylistBtn, &QPushButton::clicked, this, &MainWindow::OpenPlaylist);
    connect(m_ui->repeatBtn, &QPushButton::clicked, this, &MainWindow::ChangeRepeatMode);

    // WARN
    // connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::NextSong);

    // CONNECT SIGNALS
    connect(m_ui->addSongBtn, &QPushButton::clicked, this, &MainWindow::addSong);
    connect(m_ui->previousSongBtn, &QPushButton::clicked, m_playlist, &Playlist::prev);
    connect(m_ui->nextSongBtn, &QPushButton::clicked, m_playlist, &Playlist::next);
    connect(m_ui->playSongBtn, &QPushButton::clicked, m_playlist, &Playlist::play);
    connect(m_ui->pauseSongBtn, &QPushButton::clicked, m_playlist, &Playlist::pause);
    connect(m_ui->stopSongBtn, &QPushButton::clicked, m_playlist, &Playlist::stop);
    connect(m_ui->treeWidget, &QTreeWidget::customContextMenuRequested, this, &MainWindow::prepareMenu);
    connect(m_ui->treeWidget->selectionModel(), &QItemSelectionModel::currentRowChanged, [this](const QModelIndex &index) {
        m_playlist->setCurrent(index.row());
    });
    connect(m_playlist, &Playlist::currentSongChanged, [this](const QString& song){
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

void MainWindow::ChangeRepeatMode() {
    if (repeatMode == NoRepeat) {
        repeatMode = RepeatSong;
        UpdatePlaylist();
        qDebug() << "Repeat Song";
    } else if (repeatMode == RepeatSong) {
        repeatMode = RepeatPlaylist;
        UpdatePlaylist();
        qDebug() << "Repeat Playlist";
    } else {
        repeatMode = NoRepeat;
        UpdatePlaylist();
        qDebug() << "No Repeat";
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
        // OPEN FILE HERE
        {
            int fd = open(buffer.c_str(), O_RDONLY);

            if (fd > 0) {
                testLst.push_back(buffer);
                ::close(fd);
            }
        }

        // qDebug() << buffer.c_str();
        UpdatePlaylist();
    }
    fstream.close();
}

void MainWindow::ParseJPLAYLST(std::string filepath) {
    
}

void MainWindow::SavePlaylist() {

}

void MainWindow::ClearPlaylist() {
    // while (m_ui->playlist->count() > 0) {
    //     auto item = m_ui->playlist->takeItem(0);
    //     delete item;
    // }
}

void MainWindow::UpdatePlaylist() {
    // ClearPlaylist();
    // for (unsigned int i = selectedSong + 1; i < testLst.size(); i++) {
    //     QListWidgetItem *newItem = new QListWidgetItem;
    //     newItem->setText(testLst[i].c_str());
    //     newItem->setIcon(QIcon(QPixmap::fromImage(emptyImage)));
    //     m_ui->playlist->insertItem(i, newItem);
    // }
    // if (repeatMode == RepeatPlaylist) {
    //     for (unsigned int i = 0; i < selectedSong; i++) {
    //         QListWidgetItem *newItem = new QListWidgetItem;
    //         newItem->setText(testLst[i].c_str());
    //         newItem->setIcon(QIcon(QPixmap::fromImage(emptyImage)));
    //         m_ui->playlist->insertItem(m_ui->playlist->count(), newItem);
    //     }
    // }
}

void MainWindow::NextSong() {
    // if (repeatMode == NoRepeat) {
    //     if (selectedSong < testLst.size())
    //         selectedSong += 1;
    // }
    // else if (repeatMode == RepeatPlaylist) {
    //     selectedSong += 1;
    //     if (selectedSong >= testLst.size())
    //         selectedSong = 0;
    // }
    // UpdatePlaylist();
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
void MainWindow::prepareMenu(const QPoint & pos)
{
    QTreeWidget *tree = m_ui->treeWidget;

    QTreeWidgetItem *nd = tree->itemAt(pos);

    if (nd) {
        qDebug() << pos << nd->text(0);

        QAction *setTittle = new QAction(tr("Edit title"), this);
        QAction *setArtist = new QAction(tr("Edit artist"), this);
        QAction *setAlbum = new QAction(tr("Edit album"), this);
        QAction *setGenre = new QAction(tr("Edit genre"), this);
        
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

      QMenu menu(this);
      menu.addAction(setTittle);
      menu.addAction(setArtist);
      menu.addAction(setAlbum);
      menu.addAction(setGenre);
      menu.exec(tree->mapToGlobal(pos));
    }
}
