#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T23:23:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = TorrentDecoder
TEMPLATE = app


SOURCES += main.cpp\
        torrentdecoder.cpp \
    bencodeparser.cpp \
    metainfo.cpp

HEADERS  += torrentdecoder.h \
    bencodeparser.h \
    metainfo.h

FORMS    += torrentdecoder.ui
