#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_ui(new Ui::MainWindow) {
    m_ui->setupUi(this);

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

    connect(m_ui->loadPlaylistBtn, &QPushButton::clicked, this, &MainWindow::OpenPlaylist);
    // new QListWidgetItem(tr("Oak"), m_ui->listWidget);
}

MainWindow::~MainWindow() {
    delete m_ui;
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
        qDebug() << buffer.c_str();
    }
    fstream.close();
}

void MainWindow::ParseJPLAYLST(std::string filepath) {
    
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
