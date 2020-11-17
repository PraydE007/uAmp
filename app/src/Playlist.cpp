#include "Playlist.h"
#include "ProgressBar.h"

Playlist::Playlist(QTreeWidget* treeWidget, ProgressBar* progressBar, QSlider* slider) :
m_slider(slider),
m_progressBar(progressBar),
m_treeWidget(treeWidget)
{
    m_player = new QMediaPlayer();
    m_player->setVolume(50);
    m_slider->setValue(50);

    connect(m_slider, &QAbstractSlider::valueChanged, [this](int value) {
        m_player->setVolume(value);
    });
    connect(m_player, &QMediaPlayer::durationChanged, m_progressBar, &QProgressBar::setMaximum);
    connect(m_player, &QMediaPlayer::positionChanged, m_progressBar, &QProgressBar::setValue);
    connect(m_progressBar, &QProgressBar::valueChanged, [this](int pos) {
        m_player->setPosition(pos);
    });
    connect(m_player, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state) {
        if (state == QMediaPlayer::StoppedState) {
            if (m_mode == RepeatSong)
                m_player->play();
            else if (m_mode == RepeatPlaylist) {
                qDebug() << "Song has ended";
                this->Next();
                this->Play();
            }
        }
    });
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
//    if (m_treeWidget->topLevelItemCount() > 0 && m_mode != RepeatSong) {
//        QModelIndex modelIndex = m_treeWidget->currentIndex();
//        int row = modelIndex.row();
//
//        if (row < m_treeWidget->topLevelItemCount() - 1)
//            row++;
//        else {
//            if (m_mode != NoRepeat)
//                row = 0;
//        }
//
//        UnselectList();
//
//        if (m_mode != RepeatSong) {
//            m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
//            AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
//        } else {
//            m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(modelIndex.row()));
//            AcceptSong(m_treeWidget->topLevelItem(modelIndex.row())->text(4));
//        }
//    }
    if (m_treeWidget->topLevelItemCount() > 0) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        qDebug() << row;
        if (row < m_treeWidget->topLevelItemCount() - 1) {
            qDebug() << "There is something next";
            row++;
        } else {
            qDebug() << "The last song";
            row = 0;
        }
        qDebug() << row;
        UnselectList();
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
        AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
    }
}

void Playlist::Prev() {
    if (m_treeWidget->topLevelItemCount() > 0) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        if (row > 0) {
            row--;
        } else {
            row = m_treeWidget->topLevelItemCount() - 1;
        }
        UnselectList();
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
        AcceptSong(m_treeWidget->topLevelItem(row)->text(4));
    }
}

void Playlist::Rewind() {
    m_player->setPosition(m_player->position() - 10000);
}

void Playlist::Forward() {
    m_player->setPosition(m_player->position() + 10000);
}

void Playlist::SetCurrent(int index) {
    AcceptSong(m_treeWidget->topLevelItem(index)->text(4));
    emit CurrentSongChanged(m_treeWidget->topLevelItem(index)->text(0));

    QByteArray dataCover = getCover(m_treeWidget->topLevelItem(index)->text(4));

    if (!dataCover.isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(dataCover,"JPG");
        emit CurrentImageChanged(pixmap);
    } else {
        emit NoImage();
    }
}

void Playlist::AcceptSong(QString filepath) {
    m_player->setMedia(QUrl::fromLocalFile(filepath));
    m_progressBar->setSelected(true);
    m_progressBar->reset();
}

QWidget* Playlist::GetTreeWidget() {
    return m_treeWidget;
}

QByteArray Playlist::getCover(const QString& songPath) {
    if (songPath.isEmpty()) return QByteArray();
    TagLib::FileRef ref(QFile::encodeName(songPath).constData());

    if (ref.isNull() || !ref.file()) return QByteArray();

    // MP3
    TagLib::MPEG::File* file = dynamic_cast<TagLib::MPEG::File*>(ref.file());
    if (file && file->ID3v2Tag()) {
        TagLib::ID3v2::FrameList apic_frames =
                file->ID3v2Tag()->frameListMap()["APIC"];
        if (apic_frames.isEmpty()) return QByteArray();

        TagLib::ID3v2::AttachedPictureFrame* pic =
                static_cast<TagLib::ID3v2::AttachedPictureFrame*>(apic_frames.front());

        return QByteArray((const char*)pic->picture().data(),
                          pic->picture().size());
    }
    return QByteArray();
}
