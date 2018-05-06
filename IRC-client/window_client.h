#ifndef WINDOW_CLIENT_H
#define WINDOW_CLIENT_H

#include <QMainWindow>

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

    explicit Window_client(QWidget *parent = 0);
    virtual ~Window_client();

public slots:
   /* void slot_send_message();
    void slot_connected_to_server(const QModelIndex &index);
    void slot_disconnected_to_server(const QModelIndex &index);*/
    void slot_delete_server(const QModelIndex &index);
    void slot_delete_user(const QModelIndex &index);
    void slot_add_user();
    void slot_add_server();
    void slot_read_info_of_user();
    void slot_read_info_of_server();

private slots:
    void on_servers_doubleClicked(const QModelIndex &index);

private:
    void add_server(struct Data_for_server info);
    void add_user(QString nick);

    Window_For_Add_Server *window_for_add_server;
    Window_For_Add_User *window_for_add_user;

    Ui::Window_client *main_window;
    QList<IRC_Server*> _sockets;
    int cursor;
};

#endif // WINDOW_CLIENT_H
