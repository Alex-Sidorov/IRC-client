#pragma once

#ifndef IRC_SOCKET_H
#define IRC_SOCKET_H

#endif // IRC_SOCKET_H

#include <QTcpSocket>
#include <QMessageBox>
#include <QListWidget>
#include <QMainWindow>
#include <QDataStream>
#include <QDateTime>
#include <QTime>
#include <QFile>

#include "Data_for_server.h"

class IRC_Server : public QObject
{
    Q_OBJECT

public:
    void connect_to_server();
    void disconnect_to_server();

    int get_port()const;
    QString get_host()const;
    QString get_name()const;
    QString get_real_name()const;
    QString get_nick()const;

    void set_port(int port);
    void set_name(QString &name);
    void set_real_name(QString &real_name);
    void set_nick(QString &nick);
    void set_host(QString &host);

    void change_all_info(Data_for_server info);

    void send_message(QString message);

    bool is_connected()const;

    void add_user(QString nick);
    void delete_user(int index);

    void show_chat_server(QListWidget *chat);
    void show_users_server(QListWidget *users);

    void write_data(QFile &file);

    IRC_Server();
    IRC_Server(Data_for_server info);
    IRC_Server(const QString &name,const QString &real_name,const QString &nick,const QString &host,const int port);
    virtual ~IRC_Server();

public slots:
    void slot_connected();
    void slot_error_connected();
    void slot_recv();

signals:
    void change_chat(IRC_Server *);

private:

    QString get_time();
    void show_message(QString message);

    QTcpSocket *_socket;
    QString _host;
    int _port;
    QString _name;
    QString _real_name;
    QString _nick;
    QListWidget _users;
    QListWidget _chat;
};
