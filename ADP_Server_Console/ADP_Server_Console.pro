#-------------------------------------------------
#
# Project created by QtCreator 2014-11-19T23:53:10
#
#-------------------------------------------------

QT       += core network multimedia

QT       -= gui

TARGET = ADP_Server_Console
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    SongParser/songs.cpp \
    TcpIpServer/server.cpp

HEADERS += \
    SongParser/songs.h \
    TcpIpServer/server.h
