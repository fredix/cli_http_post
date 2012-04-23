#-------------------------------------------------
#
# Project created by QtCreator 2012-04-20T15:33:42
#
#-------------------------------------------------

QT       += core \
            network

QT       -= gui

TARGET = cli_post_http
CONFIG   += console
CONFIG   -= app_bundle
CONFIG += qxt
QXT     += core web

TEMPLATE = app


SOURCES += main.cpp

LIBS += -lQxtCore \
        -lQxtWeb


INCLUDEPATH += /usr/include/
INCLUDEPATH += /usr/include/qxt/
INCLUDEPATH += /usr/include/qxt/QxtCore
INCLUDEPATH += /usr/include/qxt/QxtWeb

HEADERS += \
    main.h
