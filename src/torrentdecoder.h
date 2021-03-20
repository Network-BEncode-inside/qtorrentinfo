#ifndef TORRENTDECODER_H
#define TORRENTDECODER_H

#include <QMainWindow>
#include <QFile>
#include <QTextStream>
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>
#include <QDebug>

namespace Ui {
class TorrentDecoder;
}

class TorrentDecoder : public QMainWindow
{
    Q_OBJECT

public:
    explicit TorrentDecoder(QWidget *parent = 0);
    ~TorrentDecoder();

private slots:
    void on_decodeFileButton_clicked();

    void on_resetButton_clicked();

    void on_quitButton_clicked();

    void on_actionOpen_triggered();

private:
    Ui::TorrentDecoder *ui;
    QFile* torrentFile;
    bool fileOpened;
    bool dataRead;
};

#endif // TORRENTDECODER_H
