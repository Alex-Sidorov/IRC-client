#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T08:42:43
#
#-------------------------------------------------

QT       += core gui
QT       +=network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IRC-client
TEMPLATE = app


SOURCES += main.cpp\
    IRC_Server.cpp \
    window_client.cpp \
    Window_For_Add_Server.cpp \
    Window_For_Add_User.cpp

HEADERS  += \
    IRC_Server.h \
    window_client.h \
    Window_For_Add_Server.h \
    Data_for_server.h \
    Window_For_Add_User.h

FORMS    += window_client.ui
