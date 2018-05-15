#ifndef WINDOW_CLIENT_H
#define WINDOW_CLIENT_H

#include <QMainWindow>
#include <QSignalMapper>
#include <QFile>

#include "IRC_Server.h"
#include "ui_window_client.h"
#include "Data_for_server.h"
#include "Window_For_Add_Server.h"
#include "Window_For_Add_User.h"

namespace Ui
{
    class Window_client;
}

class Window_client : public QMainWindow
{
    Q_OBJECT

public:

    bool read_data();
    explicit Window_client(QWidget *parent = 0);
    virtual ~Window_client();

public slots:
    void slot_send_message();
    void slot_connected_to_server(int index);
    void slot_disconnected_to_server(int index);
    void slot_sub_menu_servers(const QPoint &pos);
    void slot_sub_menu_users(const QPoint &pos);
    void slot_delete_server(int index);
    void slot_delete_user(int index);
    void slot_add_user();
    void slot_add_server();
    void slot_read_info_of_user();
    void slot_read_info_of_server();
    void slot_recv_message(IRC_Server *socket);
    void slot_error_connect(IRC_Server *socket);

private slots:
    void on_servers_doubleClicked(const QModelIndex &index);
    void on_users_doubleClicked(const QModelIndex &index);

signals:
    void error_read();

private:
    void add_server(struct Data_for_server info);
    void add_user(QString nick,bool flag_print = true);
    int find_server(IRC_Server *socket);
    void help_label();

    Window_For_Add_Server *window_for_add_server;
    Window_For_Add_User *window_for_add_user;

    Ui::Window_client *main_window;
    QList<IRC_Server*> _sockets;
    QList<QString> _box_for_users_input;

    const QString file_name;

    int cursor;
};

#endif // WINDOW_CLIENT_H
