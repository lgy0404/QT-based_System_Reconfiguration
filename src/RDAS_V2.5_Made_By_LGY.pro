#-------------------------------------------------
#
# Project created by QtCreator 2023-03-09T14:04:34
#
#-------------------------------------------------

QT       += core gui serialport\
            core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = RDAS_V2.5_Made_By_LGY
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
        widget.cpp \
    login1.cpp \
    customtabstyle.cpp \
    Ymodem.cpp \
    YmodemFileReceive.cpp \
    YmodemFileTransmit.cpp \
    md5check.cpp

HEADERS += \
        widget.h \
    login1.h \
    customtabstyle.h \
    Ymodem.h \
    YmodemFileReceive.h \
    YmodemFileTransmit.h \
    md5check.h

FORMS += \
        widget.ui \
    login1.ui

RC_ICONS = serial_iocn.ico

RESOURCES += \
    logo.qrc
