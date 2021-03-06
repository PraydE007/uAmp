#include "Playlist.h"
#include "ProgressBar.h"

Playlist::Playlist(QTreeWidget* treeWidget, ProgressBar* progressBar, QSlider* slider) :
m_slider(slider),
m_progressBar(progressBar),
m_treeWidget(treeWidget),
m_playlist(new QMediaPlaylist(this))
{
    m_player = new QMediaPlayer();
    m_player->setVolume(50);
    m_slider->setValue(50);
    m_progressBar->setTextVisible(false);

    connect(m_slider, &QAbstractSlider::valueChanged, m_player, &QMediaPlayer::setVolume);
    connect(m_player, &QMediaPlayer::durationChanged, m_progressBar, &QProgressBar::setMaximum);
    connect(m_player, &QMediaPlayer::positionChanged, m_progressBar, &QProgressBar::setValue);

    // LAMBDAS
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [this](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            if (this->GetMode() == RepeatSong) {
                m_player->setPosition(0);
                m_progressBar->setValue(0);
                this->Play();
            } else if (this->GetMode() == NoRepeat) {
                QModelIndex modelIndex = m_treeWidget->currentIndex();
                int row = modelIndex.row();

                if (row < m_treeWidget->topLevelItemCount() - 1) {
                    this->Next();
                    this->Play();
                }
            } else {
                this->Next();
                this->Play();
            }
        }
    });
    connect(m_progressBar, &QProgressBar::valueChanged, [this](qint64 pos) {
        if (m_player->position() != m_progressBar->value()) {
            m_player->setPosition(pos);
            m_pos = pos;
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

void Playlist::SelectIndex(int index) {
    QTreeWidgetItem* item = m_treeWidget->topLevelItem(index);

    m_treeWidget->topLevelItem(index)->setSelected(true);
    m_treeWidget->setCurrentItem(item);
    AcceptSong(m_treeWidget->topLevelItem(index)->text(5));
}

void Playlist::UnselectList() {
    for (int i = 0; i < m_treeWidget->topLevelItemCount(); i++)
        m_treeWidget->topLevelItem(i)->setSelected(false);
}

void Playlist::PopSong() {
    QModelIndex modelIndex = m_treeWidget->currentIndex();
    int row = modelIndex.row();

    auto item = m_treeWidget->topLevelItem(row);
    delete item;
    if (m_treeWidget->topLevelItemCount() <= 0) {
        emit CurrentSongChanged("", "");
        AcceptSong("");
        emit NoImage();
    }
}

void Playlist::Next() {
    if (m_treeWidget->topLevelItemCount() > 0) {
        QModelIndex modelIndex = m_treeWidget->currentIndex();
        int row = modelIndex.row();

        if (row < m_treeWidget->topLevelItemCount() - 1) {
            row++;
        } else {
            row = 0;
        }
        UnselectList();
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(row));
        AcceptSong(m_treeWidget->topLevelItem(row)->text(5));
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
        AcceptSong(m_treeWidget->topLevelItem(row)->text(5));
    }
}


void Playlist::Rewind() {
    m_player->setPosition(m_player->position() - 10000);
}

void Playlist::Forward() {
    m_player->setPosition(m_player->position() + 10000);
}

void Playlist::Shuffle() {
    if (m_treeWidget->topLevelItemCount() > 0) {
        std::vector<QTreeWidgetItem*> list;

        m_treeWidget->setSortingEnabled(false);
        while (m_treeWidget->topLevelItemCount() > 0)
            list.push_back(m_treeWidget->takeTopLevelItem(0));
        auto rd = std::random_device(); 
        auto rng = std::default_random_engine(rd());
        std::shuffle(list.begin(), list.end(), rng);
        for (size_t i = 0; i < list.size(); i++)
            m_treeWidget->insertTopLevelItem(i, list[i]);
        this->SetCurrent(0);
        m_treeWidget->setCurrentItem(m_treeWidget->topLevelItem(0));

    }
}

void Playlist::SetCurrent(int index) {
    AcceptSong(m_treeWidget->topLevelItem(index)->text(5));
    emit CurrentSongChanged(m_treeWidget->topLevelItem(index)->text(1), m_treeWidget->topLevelItem(index)->text(0));

    QByteArray dataCover = getCover(m_treeWidget->topLevelItem(index)->text(5));

    if (!dataCover.isEmpty()) {
        QPixmap pixmap;
        pixmap.loadFromData(dataCover,"JPG");
        emit CurrentImageChanged(pixmap);
    } else {
        emit NoImage();
    }
}

void Playlist::AcceptSongByUrl(QString url) {
    m_player->setMedia(QUrl(url));
    m_progressBar->setSelected(true);
    m_progressBar->reset();
}

void Playlist::AcceptSong(QString filepath) {
    m_player->setMedia(QUrl::fromLocalFile(filepath));
    m_progressBar->setSelected(true);
    m_progressBar->reset();
}

void Playlist::SetTime(qint64 time) { m_player->setPosition(time); }

QWidget* Playlist::GetTreeWidget() { return m_treeWidget; }
qint64 Playlist::GetPosition() { return m_player->position(); }
const char* Playlist::GetUrl() { return m_player->media().request().url().toString().toUtf8().constData(); }

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
