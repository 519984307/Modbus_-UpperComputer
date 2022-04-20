#-------------------------------------------------
#
# Project created by QtCreator 2020-06-17T10:26:47
#
#-------------------------------------------------

QT       += core gui serialbus serialport network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = prefeed
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


SOURCES += \
        main.cpp \
        prefeed.cpp \
    modbusslave.cpp \
    common.cpp \
    monitorthread.cpp \
    qtredis.cpp \
    reader.cpp

HEADERS += \
        prefeed.h \
    modbusslave.h \
    common.h \
    monitorthread.h \
    qtredis.h \
    reader.h

FORMS += \
        prefeed.ui

CONFIG += C++11
