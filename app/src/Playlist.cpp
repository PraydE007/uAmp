#include "Playlist.h"

Playlist::Playlist(QTreeWidget* treeWidget) : m_treeWidget(treeWidget) {
    m_player = new QMediaPlayer();
    m_player->setVolume(100);
}

void Playlist::Play() {
    m_player->play();
}

void Playlist::Pause() {
    m_player->pause();
}

void Playlist::Stop() {
    m_player->stop();
}

void Playlist::ClearPlaylist() {
    m_treeWidget->clear();
}

void Playlist::UnselectList() {
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); i++) {
        m_treeWidget->topLevelItem(i)->setSelected(false);
    }
}

void Playlist::Del(const QString& filePath) {
    // m_songs.erase(std::find(m_songs.begin(), m_songs.end(), filePath));
}

void Playlist::Next() {
    if (m_treeWidget->topLevelItemCount() > 0) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        if (row < m_treeWidget->topLevelItemCount() - 1)
            row++;
        else
            row = 0;
        UnselectList();
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
        AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
        // m_treeWidget->topLevelItem(row)->setSelected(true);
    }
}

void Playlist::Prev() {
    if (m_treeWidget->topLevelItemCount() > 0) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        if (row > 0)
            row--;
        else
            row = m_treeWidget->topLevelItemCount() - 1;
        UnselectList();
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
        AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
        // m_treeWidget->topLevelItem(row)->setSelected(true);
    }
}

void Playlist::SetCurrent(int index) {
    AcceptSong(m_treeWidget->topLevelItem(index)->text(4));
    emit CurrentSongChanged(m_treeWidget->topLevelItem(index)->text(0));
}

void Playlist::AcceptSong(QString filepath) {
    m_player->setMedia(QUrl::fromLocalFile(filepath));
}

QWidget* Playlist::GetTreeWidget() {
    return m_treeWidget;
}
