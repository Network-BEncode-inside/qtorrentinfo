#-------------------------------------------------
#
# Project created by QtCreator 2014-05-03T23:23:03
#
#-------------------------------------------------
CONFIG -= debug_and_release debug

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = qtorrentinfo
TEMPLATE = app


SOURCES += src/main.cpp\
        src/torrentdecoder.cpp \
    src/bencodeparser.cpp \
    src/metainfo.cpp

HEADERS  += src/torrentdecoder.h \
    src/bencodeparser.h \
    src/metainfo.h

FORMS    += src/torrentdecoder.ui
