#include "musicplayerui.h"

MusicPlayerUI::MusicPlayerUI(QWidget* parent) : QWidget(parent), usedVolume(50), ui(new Ui::MusicPlayerUI) 
{
    ui->setupUi(this);
    connect(ui->playButton, &QPushButton::clicked, this, &MusicPlayerUI::onPlayButtonClicked);
    connect(ui->nextButton, &QPushButton::clicked, this, &MusicPlayerUI::onNextButtonClicked);
    connect(ui->previousButton, &QPushButton::clicked, this, &MusicPlayerUI::onPreviousButtonClicked);
    connect(ui->positionSlider, &QSlider::sliderReleased, this, &MusicPlayerUI::onPositionSliderMoved);
    connect(ui->volumeSlider, &QSlider::valueChanged, this, &MusicPlayerUI::onVolumeSliderValueChanged);
    connect(ui->mutedButton, &QPushButton::clicked, this, &MusicPlayerUI::onSetMutedChanged);
    connect(ui->playModeButton, &QPushButton::clicked, this, &MusicPlayerUI::onPlayModeIndexChanged);
    lyricWidget = new LyricWidget(ui);
    ui->volumeSlider->setValue(50);
}

MusicPlayerUI::~MusicPlayerUI()
{
    delete ui;
}

LyricWidget* MusicPlayerUI::getLysic() {
    return lyricWidget;
}

void MusicPlayerUI::onPlayButtonClicked() {
    emit playClicked();
}

void MusicPlayerUI::onNextButtonClicked() {
    emit nextClicked();
}

void MusicPlayerUI::onPreviousButtonClicked() {
    emit previousClicked();
}

void MusicPlayerUI::onPositionSliderMoved() {

    emit sliderMoved(static_cast<qint64>(ui->positionSlider->value()));
}

void MusicPlayerUI::onVolumeSliderValueChanged(qint64 volume) {
    ui->volumelabel->setText(QString::number(volume));
    emit volumeChanged(volume);
}

void MusicPlayerUI::onPlayModeIndexChanged() {
    emit modeChanged();
}

Ui::MusicPlayerUI* MusicPlayerUI::getUi() {
    return ui;
}

void MusicPlayerUI::onSetMutedChanged() {
    int volume = ui->volumeSlider->value();
    bool muted = volume == 0;

    if (!muted) {
        // 静音
        if (usedVolume != 0) {
            usedVolume = volume; // 仅在非静音状态保存音量
        }
        ui->volumeSlider->setValue(0); // 将音量设置为 0
        ui->mutedButton->setIcon(QIcon("./images/volume_close.png"));
        emit volumeChanged(0);
    }
    else {
        // 取消静音，恢复之前的音量
        ui->mutedButton->setIcon(QIcon("./images/volume.png"));
        ui->volumeSlider->setValue(usedVolume);
        emit volumeChanged(usedVolume);
    }
}
