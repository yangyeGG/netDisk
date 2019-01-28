#-------------------------------------------------
#
# Project created by QtCreator 2018-11-20T15:31:58
#
#-------------------------------------------------

QT       += core gui network xml axcontainer

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = yy_NetDisk
TEMPLATE = app

LIBS += D:\Qt\qt\Tools\mingw530_32\i686-w64-mingw32\lib\libws2_32.a
SOURCES += main.cpp\
        login.cpp \
    xmb.cpp \
    md5.c \
    sign_up.cpp \
    frmmain.cpp \
    waitdig.cpp \
    qnavigationwidget.cpp \
    mysocket.cpp \
    myxml.cpp \
    sendthread.cpp \
    recvthread.cpp \
    mysystemtray.cpp

HEADERS  += login.h \
    xmb.h \
    md5.h \
    sign_up.h \
    NetDisk_Type.h \
    frmmain.h \
    waitdig.h \
    qnavigationwidget.h \
    mysocket.h \
    myxml.h \
    sendthread.h \
    recvthread.h \
    mynet_def.h \
    mysystemtray.h

FORMS    += login.ui \
    xmb.ui \
    sign_up.ui \
    frmmain.ui \
    waitdig.ui

RESOURCES += \
    img.qrc \
    rc.qrc

RC_FILE = myico.rc
