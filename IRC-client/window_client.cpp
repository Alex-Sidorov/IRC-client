#include "window_client.h"

#include <QDebug>

Window_client::Window_client(QWidget *parent) : QMainWindow(parent)
{
    main_window = new Ui::Window_client;

    main_window->setupUi(this);
    connect(main_window->addServer,SIGNAL(clicked()),SLOT(slot_add_server()));
    connect(main_window->addUser,SIGNAL(clicked()),SLOT(slot_add_user()));

    window_for_add_server = new Window_For_Add_Server;
    connect(window_for_add_server,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_server()));

    window_for_add_user = new Window_For_Add_User;
    connect(window_for_add_user,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_user()));

    cursor = 0;
}

void Window_client::slot_read_info_of_server()
{
    Data_for_server info = window_for_add_server->get_info();
    add_server(info);
    window_for_add_server->close_window();
}

void Window_client::slot_read_info_of_user()
{
    QString nick = window_for_add_user->get_info();
    add_user(nick);
    window_for_add_user->close_window();
}

void Window_client::slot_add_server()
{
    window_for_add_server->clear_window();
    window_for_add_server->open_window();
}

void Window_client::slot_add_user()
{
    window_for_add_user->clear_window();
    window_for_add_user->open_window();
}

void Window_client::add_server(struct Data_for_server info)
{
    IRC_Server *socket = new IRC_Server(info);
    _sockets+=socket;
    main_window->servers->addItem(socket->get_host());
}

void Window_client::add_user(QString nick)
{
    if(main_window->servers->count()!=0)
    {
        main_window->users->addItem(nick);
        _sockets.at(cursor)->add_user(nick);
    }

}

void Window_client::slot_delete_server(const QModelIndex &index)
{
    main_window->servers->removeItemWidget(main_window->servers->item(index.row()));
}

void Window_client::slot_delete_user(const QModelIndex &index)
{
    _sockets.at(index.row())->delete_user(index.row());
    main_window->users->removeItemWidget(main_window->users->item(index.row()));
}

Window_client::~Window_client()
{
    IRC_Server *temp;
    while(_sockets.size()!=0)
    {
        temp = _sockets.at(0);
        delete temp;
        _sockets.erase(_sockets.begin());
    }

    if(window_for_add_server)
    {
        delete window_for_add_server;
    }

    if(window_for_add_user)
    {
        delete window_for_add_user;
    }
    delete main_window;
}

void Window_client::on_servers_doubleClicked(const QModelIndex &index)
{
        if(!index.isValid() || cursor==index.row())
        {
            return;
        }

        cursor = index.row();

        _sockets.at(cursor)->show_chat_server(main_window->chat);
        _sockets.at(cursor)->show_users_server(main_window->users);

}
