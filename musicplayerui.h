#ifndef __MUSIC_PLAYER_UI_H__
#define __MUSIC_PLAYER_UI_H__

// View - ����������

#include "common.h"
#include "lyricsdisplaywidget.h"
#include "x64/Debug/uic/ui_musicplayer.h"

class MusicPlayerUI : public QWidget {
    Q_OBJECT

public:
    explicit MusicPlayerUI(QWidget* parent = nullptr);
    Ui::MusicPlayerUI* getUi();
    LyricWidget* getLysic();
    ~MusicPlayerUI();

signals:
    void playClicked();
    void nextClicked();
    void previousClicked();
    void sliderMoved(qint64 position);
    void volumeChanged(qint64 volume);
    void modeChanged();

public slots:
    void onPlayButtonClicked();
    void onNextButtonClicked();
    void onPreviousButtonClicked();
    void onPositionSliderMoved();
    void onVolumeSliderValueChanged(qint64 volume);
    void onSetMutedChanged();
    void onPlayModeIndexChanged();

private:
    LyricWidget* lyricWidget;
    Ui::MusicPlayerUI* ui;  // ���� UI ָ��
    int usedVolume; // �����ָ�����
};

#endif // __MUSIC_PLAYER_UI_H__
