#include "torrentdecoder.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TorrentDecoder w;
    w.show();

    return a.exec();
}
