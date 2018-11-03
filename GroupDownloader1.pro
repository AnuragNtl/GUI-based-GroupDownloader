#-------------------------------------------------
#
# Project created by QtCreator 2017-02-14T19:22:53
#
#-------------------------------------------------

QT       += core gui
QT       += network
QT       += xml
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GroupDownloader1
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    serverui.cpp \
    clientui.cpp \
    downloader.cpp \
    partialdownloader.cpp \
    distribution.cpp \
    infodata.cpp \
    distributionclient.cpp \
    addclient.cpp \
    dividerange.cpp \
    merge.cpp

HEADERS  += mainwindow.h \
    serverui.h \
    clientui.h \
    merge.h \
    distribution.h \
    downloader.h \
    partialdownloader.h \
    distributionreceiver.h \
    infodata.h \
    distributionclient.h \
    addclient.h \
    dividerange.h

FORMS    += \
    serverui.ui \
    clientui.ui \
    mainwindow.ui \
    addclient.ui \
    dividerange.ui

DISTFILES +=
