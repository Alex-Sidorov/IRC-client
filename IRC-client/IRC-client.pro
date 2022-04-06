#-------------------------------------------------
#
# Project created by QtCreator 2018-03-18T08:42:43
#
#-------------------------------------------------

QT += core gui
QT += network
QT += widgets

TEMPLATE = app


SOURCES += main.cpp\
    IRC_Server.cpp \
    Window_For_Add_Channel.cpp \
    window_client.cpp \
    Window_For_Add_Server.cpp \
    Window_For_Add_User.cpp \
    Parser.cpp

HEADERS  += \
    IRC_Server.h \
    Window_For_Add_Channel.h \
    ui_window_client.h \
    window_client.h \
    Window_For_Add_Server.h \
    Data_for_server.h \
    Window_For_Add_User.h \
    Parser.h

FORMS    += window_client.ui
