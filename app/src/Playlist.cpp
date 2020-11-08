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

void Playlist::ChangeRepeatMode() {
    if (m_mode == NoRepeat)
        m_mode = RepeatSong;
    else if (m_mode == RepeatSong)
        m_mode = RepeatPlaylist;
    else
        m_mode = NoRepeat;
}

Playlist::RepeatMode Playlist::GetMode() { return m_mode; }

void Playlist::ClearPlaylist() {
    m_treeWidget->clear();
}

void Playlist::UnselectList() {
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); i++) {
        m_treeWidget->topLevelItem(i)->setSelected(false);
    }
}

void Playlist::PopSong() {
    QModelIndex modelIndex = m_treeWidget->currentIndex();
    int row = modelIndex.row();

    auto item = m_treeWidget->topLevelItem(row);
    delete item;
    // m_player->setMedia(nullptr);
    if (m_treeWidget->topLevelItemCount() == 0)
        emit CurrentSongChanged("");
}

void Playlist::Next() {
    if (m_treeWidget->topLevelItemCount() > 0 && m_mode != RepeatSong) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        if (row < m_treeWidget->topLevelItemCount() - 1)
            row++;
        else {
            if (m_mode != NoRepeat)
                row = 0;
        }

        UnselectList();

        if (m_mode != RepeatSong) {
            m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
            AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
        } else {
            m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(modelIndex.row()));
            AcceptSong(m_treeWidget->topLevelItem(modelIndex.row())->text(4));
        }
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
