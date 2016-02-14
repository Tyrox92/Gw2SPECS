#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T02:45:47
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
TARGET = Gw2DPS
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    imagereader.cpp \
    imageattributes.cpp \
    charactergrid.cpp \
    characterpattern.cpp \
    screenrecorder.cpp \
    dmgmeter.cpp \
    configurator.cpp \
    settings.cpp \
    mydialog.cpp \
    global.cpp

HEADERS  += mainwindow.h \
    imagereader.h \
    imageattributes.h \
    charactergrid.h \
    characterpattern.h \
    screenrecorder.h \
    dmgmeter.h \
    configurator.h \
    settings.h \
    global.h \
    mydialog.h

FORMS    += mainwindow.ui \
    configurator.ui \
    mydialog.ui

RESOURCES += \
    Icons/Icons.qrc

RC_FILE = jaxnX.rc
