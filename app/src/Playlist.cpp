#include "Playlist.h"

Playlist::Playlist(QTreeWidget* treeWidget) :
m_treeWidget(treeWidget)
{
    m_player = new QMediaPlayer();
    m_player->setVolume(100);
}

void Playlist::play() {
    m_player->play();
}

void Playlist::pause() {
    m_player->pause();
}

void Playlist::stop() {
    m_player->stop();
}

void Playlist::del(const QString& filePath) {
    m_songs.erase(std::find(m_songs.begin(), m_songs.end(), filePath));
}

void Playlist::next() {
    if (m_currentSong != std::prev(m_songs.end())) {
        m_currentSong = std::next(m_currentSong);
        m_player->setMedia(QUrl::fromLocalFile(m_treeWidget.child(m_currentRow).text(4)));
        emit currentSongChanged(m_treeWidget.child(m_currentRow).text(0));
    }
}

void Playlist::prev() {
    if(m_currentSong != m_songs.begin()) {
        m_currentSong = std::prev(m_currentSong);
        m_player->setMedia(QUrl::fromLocalFile(m_treeWidget.child(m_currentRow).text(4)));
        emit currentSongChanged(m_treeWidget.child(m_currentRow).text(0));
    }
}

void Playlist::setCurrent(int index) {
    m_currentSong = std::find(m_songs.begin(), m_songs.end(), m_songs.at(index));
    m_player->setMedia(QUrl::fromLocalFile(*m_currentSong));
    emit currentSongChanged(*m_currentSong);
}

QWidget* Playlist::getTreeWidget() {
    return m_treeWidget;
}
