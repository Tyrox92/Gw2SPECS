#-------------------------------------------------
#
# Project created by QtCreator 2015-02-21T02:45:47
#
#-------------------------------------------------

QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

#CONFIG += c++11
QMAKE_CXXFLAGS += -std=c++0x
QMAKE_CXXFLAGS_WARN_ON += -Wno-reorder
TARGET = Gw2SPECS
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
    global.cpp \
    qcustomplot.cpp \
    firststart.cpp \
    combatmode.cpp \
    connectionfailed.cpp \
    savelog.cpp \
    updatecheck.cpp \
    authenticate.cpp \
    startserver.cpp

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
    mydialog.h \
    qcustomplot.h \
    mumblelink.h \
    firststart.h \
    combatmode.h \
    connectionfailed.h \
    savelog.h \
    updatecheck.h \
    authenticate.h \
    startserver.h

FORMS    += mainwindow.ui \
    configurator.ui \
    mydialog.ui \
    firststart.ui \
    combatmode.ui \
    connectionfailed.ui \
    savelog.ui \
    updatecheck.ui \
    authenticate.ui \
    startserver.ui

RESOURCES += \
    Icons/Icons.qrc

RC_FILE = specs.rc

TRANSLATIONS+=  languages/gw2specs_English.ts
TRANSLATIONS+=  languages/gw2specs_Deutsch.ts
TRANSLATIONS+=  languages/gw2specs_Francais.ts
