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
//    m_songs.erase(std::find(m_songs.begin(), m_songs.end(), filePath));
}

void Playlist::next() {
    if (m_currentRow < m_treeWidget->topLevelItemCount() - 1) {
        m_currentRow++;
        m_player->setMedia(QUrl::fromLocalFile(m_treeWidget->topLevelItem(m_currentRow)->text(4)));
        emit currentSongChanged(m_treeWidget->topLevelItem(m_currentRow)->text(0));
    }
}

void Playlist::prev() {
    if(m_currentRow > 0) {
        m_currentRow--;
        m_player->setMedia(QUrl::fromLocalFile(m_treeWidget->topLevelItem(m_currentRow)->text(4)));
        emit currentSongChanged(m_treeWidget->topLevelItem(m_currentRow)->text(0));
    }
}

void Playlist::setCurrent(int index) {
    m_currentRow = index;
    m_player->setMedia(QUrl::fromLocalFile(m_treeWidget->topLevelItem(index)->text(4)));
    emit currentSongChanged(m_treeWidget->topLevelItem(index)->text(0));
}

QWidget* Playlist::getTreeWidget() {
    return m_treeWidget;
}
