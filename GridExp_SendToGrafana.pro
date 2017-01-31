#-------------------------------------------------
#
# Project created by QtCreator 2017-01-06T18:02:53
#
#-------------------------------------------------

QT       += core gui network charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = GridExp_SendToGrafana
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    serverconfigures.cpp

HEADERS  += mainwindow.h \
    serverconfigures.h

FORMS    += mainwindow.ui

CONFIG   += c++11
QMAKE_CXXFLAGS += -std=c++11

RESOURCES +=

INCLUDEPATH += qtcsv/include \
        qtcsv

#!msvc {
    # flags for gcc-like compiler
#    CONFIG += warn_on
#    QMAKE_CXXFLAGS_WARN_ON += -Werror -Wformat=2 -Wuninitialized -Winit-self \
#            -Wmissing-include-dirs -Wswitch-enum -Wundef -Wpointer-arith \
#            -Wdisabled-optimization -Wcast-align -Wcast-qual
#}

DEFINES += QTCSV_LIBRARY

SOURCES += \
    qtcsv/sources/writer.cpp \
    qtcsv/sources/variantdata.cpp \
    qtcsv/sources/stringdata.cpp \
    qtcsv/sources/reader.cpp \
    qtcsv/sources/contentiterator.cpp

HEADERS += \
    qtcsv/include/qtcsv/qtcsv_global.h \
    qtcsv/include/qtcsv/writer.h \
    qtcsv/include/qtcsv/variantdata.h \
    qtcsv/include/qtcsv/stringdata.h \
    qtcsv/include/qtcsv/reader.h \
    qtcsv/include/qtcsv/abstractdata.h \
    qtcsv/sources/filechecker.h \
    qtcsv/sources/contentiterator.h \
    qtcsv/sources/symbols.h

DISTFILES += \
    settings_new.conf


