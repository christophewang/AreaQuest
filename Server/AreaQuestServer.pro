#-------------------------------------------------
#
# Project created by QtCreator 2013-04-15T15:36:21
#
#-------------------------------------------------

QT       += core
QT       += network
QT       += sql
QT       -= gui

TARGET = AreaQuestServer
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += static

TEMPLATE = app

SOURCES += \
    Network.cpp \
    Database.cpp \
    Main.cpp \
    AccountManager.cpp \
    CharacterInfo.cpp \
    CharacterMap.cpp \
    CharacterBattle.cpp \
    CharacterMessage.cpp \
    AreaStrings.cpp \
    Utils.cpp \
    CharacterRanking.cpp

HEADERS += \
    Network.h \
    Database.h \
    AccountManager.h \
    CharacterInfo.h \
    CharacterMap.h \
    CharacterBattle.h \
    CharacterMessage.h \
    AreaStrings.h \
    Utils.h \
    CharacterRanking.h
