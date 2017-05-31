#-------------------------------------------------
#
# Project created by QtCreator 2017-05-29T15:14:25
#
#-------------------------------------------------

QT       += core gui
QT += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = air_condition_v2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    open.cpp \
    login.cpp \
    json_handle.cpp \
    jsonsocket.cpp \
    readthread.cpp \
    secondair.cpp \
    getfreshrate.cpp \
    getkwhbill.cpp \
    receivemode.cpp

HEADERS  += widget.h \
    open.h \
    login.h \
    json_handle.h \
    jsonsocket.h \
    readthread.h \
    secondair.h

FORMS    += widget.ui \
    open.ui \
    login.ui

RESOURCES += \
    picture.qrc
