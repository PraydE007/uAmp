#ifndef UAMP_PROGRESSBAR_H
#define UAMP_PROGRESSBAR_H

#include "q_includes.h"

class ProgressBar : public QProgressBar {
    Q_OBJECT
public:
    explicit ProgressBar(QWidget* parent = nullptr);

    void setSelected(bool isSongSelected);

signals:
protected:
    bool event(QEvent * event) override;

private:
    bool m_mousePressed{false};
    bool m_songSelected{false};
};

#endif //UAMP_PROGRESSBAR_H
