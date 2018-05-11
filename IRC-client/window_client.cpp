#include "window_client.h"

#include <QDebug>

Window_client::Window_client(QWidget *parent) : QMainWindow(parent),file_name("data")
{
    main_window = new Ui::Window_client;

    this->setFixedSize(this->maximumSize());

    main_window->setupUi(this);

    connect(main_window->addServer,SIGNAL(clicked()),SLOT(slot_add_server()));
    connect(main_window->addUser,SIGNAL(clicked()),SLOT(slot_add_user()));

    window_for_add_server = new Window_For_Add_Server;
    connect(window_for_add_server,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_server()));

    window_for_add_user = new Window_For_Add_User;
    connect(window_for_add_user,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_user()));

    main_window->servers->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(main_window->servers,SIGNAL(customContextMenuRequested(QPoint)),SLOT(slot_sub_menu_servers(QPoint)));

    main_window->users->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(main_window->users,SIGNAL(customContextMenuRequested(QPoint)),SLOT(slot_sub_menu_users(QPoint)));

    connect(main_window->field_for_input_text,SIGNAL(returnPressed()),SLOT(slot_send_message()));

    cursor = 0;
}

bool Window_client::read_data()
{
    QFile file(file_name);
    Data_for_server data;
    int count_users = 0;
    QString name_user;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("ERROR"), tr("Cannot read file."));
        return false;
    }

     QDataStream stream(&file);
     while(!stream.atEnd())
     {
            stream>>data.host;
            stream>>data.port;
            stream>>data.name;
            stream>>data.real_name;
            stream>>data.nick;

            add_server(data);

            stream>>count_users;

            for(int i=0; i<count_users; i++)
            {
                stream>>name_user;
                add_user(name_user, cursor==0?true:false);
            }
            cursor++;
     }
     file.close();
     cursor = 0;
     return true;
}


void Window_client::slot_sub_menu_servers(const QPoint &pos)
{
    QPoint item_pos = main_window->servers->mapToGlobal(pos);
    QMenu sub_menu;

    int index_item = main_window->servers->indexAt(pos).row();
    if(index_item==-1)
    {
        return;
    }

    QSignalMapper *signal_map_one = new QSignalMapper;
    QSignalMapper *signal_map_two = new QSignalMapper;

    QAction *action_one;
    QAction *action_two;

    if(_sockets.at(index_item)->is_connected())
    {
        action_one = sub_menu.addAction("Disconnect");
        connect(action_one,SIGNAL(triggered()),signal_map_one,SLOT(map()));
        signal_map_one->setMapping(action_one,index_item);
        connect(signal_map_one,SIGNAL(mapped(int)),SLOT(slot_disconnected_to_server(int)));
    }
    else
    {
        action_one = sub_menu.addAction("Connect");
        connect(action_one,SIGNAL(triggered()),signal_map_one,SLOT(map()));
        signal_map_one->setMapping(action_one,index_item);
        connect(signal_map_one,SIGNAL(mapped(int)),SLOT(slot_connected_to_server(int)));
    }


    action_two = sub_menu.addAction("Delete");
    connect(action_two,SIGNAL(triggered()),signal_map_two,SLOT(map()));
    signal_map_two->setMapping(action_two,index_item);
    connect(signal_map_two,SIGNAL(mapped(int)),SLOT(slot_delete_server(int)));

    sub_menu.exec(item_pos);

    delete signal_map_one;
    delete signal_map_two;

}

void Window_client::slot_sub_menu_users(const QPoint &pos)
{
    QPoint item_pos = main_window->users->mapToGlobal(pos);
    QMenu sub_menu;

    int index_item = main_window->users->indexAt(pos).row();
    if(index_item==-1)
    {
        return;
    }

    QSignalMapper *signal_map_one = new QSignalMapper;

    QAction *action_one;

    action_one = sub_menu.addAction("Delete");
    connect(action_one,SIGNAL(triggered()),signal_map_one,SLOT(map()));
    signal_map_one->setMapping(action_one,index_item);
    connect(signal_map_one,SIGNAL(mapped(int)),SLOT(slot_delete_user(int)));

    sub_menu.exec(item_pos);

    delete signal_map_one;
}

void Window_client::slot_connected_to_server(int index)
{
    _sockets.at(index)->connect_to_server();
}

void Window_client::slot_disconnected_to_server(int index)
{
    _sockets.at(index)->disconnect_to_server();
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
    main_window->statusBar->clearMessage();
    window_for_add_server->clear_window();
    window_for_add_server->open_window();
}

void Window_client::slot_add_user()
{
    if(main_window->servers->count()!=0)
    {
        window_for_add_user->clear_window();
        window_for_add_user->open_window();
    }
    else
    {
        main_window->statusBar->showMessage("Servers isn't.");
    }
}

void Window_client::add_server(struct Data_for_server info)
{
    IRC_Server *socket = new IRC_Server(info);
    _sockets+=socket;
    main_window->servers->addItem(socket->get_host());
    connect(socket,SIGNAL(change_chat(IRC_Server *)),this,SLOT(slot_recv_message(IRC_Server *)));
}

void Window_client::slot_recv_message(IRC_Server *socket)
{
    if(socket == _sockets.at(cursor))
    {
        _sockets.at(cursor)->show_chat_server(main_window->chat);
    }
}

void Window_client::slot_send_message()
{
    if(main_window->servers->count()==0)
    {
        main_window->statusBar->showMessage("Servers isn't.");
    }
    else if(!_sockets.at(cursor)->is_connected())
    {
        main_window->statusBar->showMessage("Servers is disconnect.");
    }
    else if(main_window->field_for_input_text->text().isEmpty())
    {
        main_window->statusBar->showMessage("Text is empty.");
    }
    else
    {
        main_window->statusBar->clearMessage();
        _sockets.at(cursor)->send_message(main_window->field_for_input_text->text());
        main_window->field_for_input_text->clear();
    }
}

void Window_client::add_user(QString nick, bool flag_print)
{
    if(main_window->servers->count()!=0)
    {
        if(flag_print == true)
        {
             main_window->users->addItem(nick);
        }
        _sockets.at(cursor)->add_user(nick);
    }
}

void Window_client::slot_delete_server(int index)
{
    IRC_Server *item = _sockets.takeAt(index);
    delete item;
    main_window->servers->takeItem(index);
    if(cursor==index)
    {
        if(main_window->servers->count()==0)
        {
            main_window->users->clear();
            main_window->chat->clear();
            cursor = 0;
        }
        else
        {
            cursor = 0;
            _sockets.at(cursor)->show_chat_server(main_window->chat);
            _sockets.at(cursor)->show_users_server(main_window->users);
        }
    }
}

void Window_client::slot_delete_user(int index)
{
    _sockets.at(cursor)->delete_user(index);
    main_window->users->takeItem(index);
}

Window_client::~Window_client()
{
    IRC_Server *temp;

    QFile file(file_name);
    if(!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this,tr("ERROR"), tr("Cannot write file."));
        while(_sockets.size()!=0)
        {
            temp = _sockets.at(0);
            delete temp;
            _sockets.erase(_sockets.begin());
        }
    }
    else
    {
        while(_sockets.size()!=0)
        {
            temp = _sockets.at(0);
            temp->write_data(file);

            delete temp;
            _sockets.erase(_sockets.begin());
        }
        file.close();
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
