#-------------------------------------------------
#
# Project created by QtCreator 2017-11-21T14:01:57
#
#-------------------------------------------------

QT       += core gui \
         multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tower-defence
TEMPLATE = app

CONFIG += c++11
CONFIG += release
CONFIG -= debug

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    bullet.cpp \
    enemy.cpp \
    game.cpp \
    main.cpp \
    tower.cpp \
    buildtowericon.cpp \
    highscore.cpp \
    button.cpp \
    map.cpp \
    mapeditor.cpp

HEADERS += \
    bullet.h \
    enemy.h \
    game.h \
    tower.h \
    buildtowericon.h \
    highscore.h \
    button.h \
    constants.h \
    map.h \
    mapeditor.h \
    maptypes.h

release: DESTDIR = build/release
debug:   DESTDIR = build/debug

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

FORMS +=

RESOURCES += \
    res.qrc
