#ifndef LYRICWIDGET_H
#define LYRICWIDGET_H

// view - 歌词视图

#include "common.h"
#include "./x64/Debug/uic/ui_musicplayer.h"


class LyricLine {
public:
    qint64 time;
    QString text;
    LyricLine(qint64 time, QString text) : time(time), text(text) {}
    bool operator <(const LyricLine& other) const {
        return time < other.time;
    }
};

class LyricWidget{
public:
    LyricWidget(Ui::MusicPlayerUI* ui);

    bool processLyricsFile(const QString& filePath);
    int getIndex(qint64 position);
    void updateLyrics(qint64 position);
    void show(qint64 position);
    QString getLyricText(int index);
    void clear();

private:
    Ui::MusicPlayerUI* ui;  // 定义 UI 指针
    QList<LyricLine> lines;
};

#endif // LYRICWIDGET_H
