#include "lyricsdisplaywidget.h"


LyricWidget::LyricWidget(Ui::MusicPlayerUI* ui) :
    ui(ui)
{
    clear();//��ղ�������(.ui�ļ���)
}


bool LyricWidget::processLyricsFile(const QString& filePath) {
    QFile lyricFile;
    // ����Ƿ������·������ "." ��ͷ��
    if (filePath[0] != '.') {
        // ����������·�����򹹽�����·��
        QString fullPath = "./localcache" + filePath;
        lyricFile.setFileName(fullPath);
    }
    else {
        // ��������·������ʹ�����·������ QFile ����
        lyricFile.setFileName(filePath);
    }
    if (!lyricFile.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Failed to open lyrics file:" << filePath;
        return false;
    }

    QTextStream in(&lyricFile);
    in.setCodec("UTF-8"); // �����ַ�����

    lines.clear(); // ���֮ǰ�ĸ��

    const QRegExp timeTag("\\[(\\d+):(\\d+\\.\\d+)\\]");
    while (!in.atEnd()) {
        QString line = in.readLine(); // ���ж�ȡ����ļ�����

        int pos = 0;
        while ((pos = timeTag.indexIn(line, pos)) != -1) {
            qint64 minutes = timeTag.cap(1).toInt();
            qreal seconds = timeTag.cap(2).toDouble();
            qint64 time = static_cast<qint64>((minutes * 60 + seconds) * 1000);

            QString text;
            int nextPos = timeTag.indexIn(line, pos + timeTag.matchedLength());
            if (nextPos != -1) {
                text = line.mid(pos + timeTag.matchedLength(), nextPos - (pos + timeTag.matchedLength())).trimmed();
            }
            else {
                text = line.mid(pos + timeTag.matchedLength()).trimmed();
            }

            if (!text.isEmpty()) {
                this->lines.append(LyricLine(time, text));
            }

            pos = nextPos; // �ƶ�����һ��ʱ���ǩ
        }
    }

    // ��ʱ����������
    qSort(this->lines.begin(), this->lines.end());
    for (auto& text : lines) {
        QString& lastText = lines.last().text;
        if (text.text != lastText) {
            text.text.chop(4);
        }
        else if (text.text == lastText && (lastText.right(4) == "\\r\\n")) {
            text.text.chop(4);
            int pos = text.text.lastIndexOf(']');
            if (pos != -1) {
                text.text = text.text.mid(pos + 1).trimmed();
            }
        }
        else {
            int pos = text.text.lastIndexOf(']');
            if (pos != -1) {
                text.text = text.text.mid(pos + 1).trimmed();
            }
        }
    }
    lyricFile.close();
    return true;
}


void LyricWidget::updateLyrics(qint64 position) {
    show(position);
}

int LyricWidget::getIndex(qint64 position)
{
    if (!lines.size()) {
        return -1;
    }
    else {
        if (lines[0].time >= position) {
            return 0;
        }
    }
    int i = 1;
    for (i = 1; i < lines.size(); i++) {
        if (lines[i - 1].time < position && lines[i].time >= position) {
            return i - 1;
        }
    }
    return lines.size() - 1;
}

void LyricWidget::show(qint64 position)
{
    int index = getIndex(position);
    if (index == -1) {
        ui->label_3i->setText("");
        ui->label_2i->setText("");
        ui->label_1i->setText("");
        ui->label_i->setText(u8"��ǰ�����޸��");
        ui->label_i1->setText("");
        ui->label_i2->setText("");
        ui->label_i3->setText("");
    }
    else {
        ui->label_3i->setText(getLyricText(index - 3));
        ui->label_2i->setText(getLyricText(index - 2));
        ui->label_1i->setText(getLyricText(index - 1));
        ui->label_i->setText(getLyricText(index));
        ui->label_i1->setText(getLyricText(index + 1));
        ui->label_i2->setText(getLyricText(index + 2));
        ui->label_i3->setText(getLyricText(index + 3));
    }
}

QString LyricWidget::getLyricText(int index)
{
    if (index < 0 || index >= lines.size()) {
        return "";
    }
    else {
        return lines[index].text;
    }
}

void LyricWidget::clear()
{
    ui->label_3i->setText("");
    ui->label_2i->setText("");
    ui->label_1i->setText("");
    ui->label_i->setText("");
    ui->label_i1->setText("");
    ui->label_i2->setText("");
    ui->label_i3->setText("");
}