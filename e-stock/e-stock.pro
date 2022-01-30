#-------------------------------------------------
#
# Project created by QtCreator 2020-02-13T11:27:51
#
#-------------------------------------------------

QT       += core gui sql serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = e-stock
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
        Ihm.cpp \
    Supervision.cpp \
    Communication.cpp \
    Bdd.cpp \
    Rfid.cpp \
    CodeBarre.cpp \
    Utilisateur.cpp \
    Article.cpp \
    Armoire.cpp \
    Casier.cpp \
    Keyboard.cpp

HEADERS += \
        Ihm.h \
    Supervision.h \
    Communication.h \
    Bdd.h \
    Rfid.h \
    CodeBarre.h \
    Utilisateur.h \
    Article.h \
    Armoire.h \
    Casier.h \
    Keyboard.h

FORMS += \
        Ihm.ui

# Default rules for deployment.
target.path = /home/pi
INSTALLS    += target

CONFIG(release, debug|release):DEFINES+=QT_NO_DEBUG_OUTPUT
