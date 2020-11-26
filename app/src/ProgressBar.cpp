#include "ProgressBar.h"

ProgressBar::ProgressBar(QWidget* parent) : QProgressBar(parent) {
    this->setRange(0, 100);
    this->setValue(0);
    this->setMouseTracking(true);
};

void ProgressBar::setSelected(bool isSongSelected) {
    m_songSelected = isSongSelected;
}

bool ProgressBar::event(QEvent * event) {
    if (m_songSelected) {
        if (event->type() == QEvent::MouseButtonPress) {
            QMouseEvent *pressedEvent = static_cast<QMouseEvent *>(event);
            int value = (this->maximum() * pressedEvent->pos().x()) / (this->width() - 1);
            this->reset();
            this->setValue(value);
            m_mousePressed = true;
        } else if (event->type() == QEvent::MouseButtonRelease) {
            m_mousePressed = false;
        } else if (event->type() == QEvent::MouseMove && m_mousePressed) {
            QMouseEvent *moveEvent = static_cast<QMouseEvent *>(event);
            int value = (this->maximum() * moveEvent->pos().x()) / (this->width() - 1);
            this->reset();
            this->setValue(value);
            return true;
        }
    }
    return QProgressBar::event(event);
}
