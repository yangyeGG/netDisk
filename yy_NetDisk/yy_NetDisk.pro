#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T15:31:58
#
#-------------------------------------------------

QT       += core gui network

TARGET = yy_NetDisk
TEMPLATE = app


SOURCES += main.cpp\
        login.cpp \
    xmb.cpp \
    my_qlabel.cpp \
    md5.c

HEADERS  += login.h \
    xmb.h \
    my_qlabel.h \
    md5.h

FORMS    += login.ui \
    xmb.ui

RESOURCES += \
    img.qrc
