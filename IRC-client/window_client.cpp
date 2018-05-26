#include "window_client.h"

#include <QDebug>

Window_client::Window_client(QWidget *parent) : QMainWindow(parent),file_name("data")
{
    main_window = new Ui::Window_client;
    this->setWindowTitle("IRC-Client");

    main_window->setupUi(this);

    connect(main_window->addServer,SIGNAL(clicked()),SLOT(slot_add_server()));
    connect(main_window->addUser,SIGNAL(clicked()),SLOT(slot_add_user()));
    connect(main_window->add_channel,SIGNAL(clicked()),SLOT(slot_add_channel()));

    window_for_add_server = new Window_For_Add_Server;
    connect(window_for_add_server,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_server()));
    connect(window_for_add_server,SIGNAL(change_info(int)),this,SLOT(slot_change_info_of_server(int)));

    window_for_add_user = new Window_For_Add_User;
    connect(window_for_add_user,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_user()));
    connect(window_for_add_user,SIGNAL(change_info(int)),this,SLOT(slot_change_info_of_user(int)));

    window_for_add_channel = new Window_For_Add_Channel;
    connect(window_for_add_channel,SIGNAL(entry_info()),this,SLOT(slot_read_info_of_channel()));
    connect(window_for_add_channel,SIGNAL(change_info(int)),this,SLOT(slot_change_info_of_channel(int)));

    main_window->servers->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(main_window->servers,SIGNAL(customContextMenuRequested(QPoint)),SLOT(slot_sub_menu_servers(QPoint)));

    main_window->users->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(main_window->users,SIGNAL(customContextMenuRequested(QPoint)),SLOT(slot_sub_menu_users(QPoint)));

    main_window->channels->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(main_window->channels,SIGNAL(customContextMenuRequested(QPoint)),SLOT(slot_sub_menu_channels(QPoint)));

    connect(main_window->field_for_input_text,SIGNAL(returnPressed()),SLOT(slot_send_message()));

    help_label();

    cursor = 0;
}

void Window_client::slot_change_channel(int index)
{

    window_for_add_channel->fill_form(main_window->channels->item(index)->text(),index);
    window_for_add_channel->close_window();
    window_for_add_channel->open_window();
}

void Window_client::slot_change_user(int index)
{
    window_for_add_user->fill_form(main_window->users->item(index)->text(),index);
    window_for_add_user->close_window();
    window_for_add_user->open_window();
}

void Window_client::slot_change_server(int index)
{
    window_for_add_server->fill_form(_sockets.at(index)->get_info(),index);
    window_for_add_server->open_window();
}

void Window_client::help_label()
{
    QString help;
    help+="ADMIN [<сервер>]- Cервер вернет информацию о сервере\n";
    help+="AWAY [<сообщение>]- Задаёт сообщение, которое автоматически отображается в ответ на приватные сообщения\n";
    help+="DIE - Заставить сервер отключиться\n";
    help+="INFO [<Цель>] - Возвращает информацию о сервере, определённом в параметре <цель>, или текущем сервере, если параметр <цель> не указан.\n";
    help+="INVITE <пользователь> <канал> - Приглашает пользователя <пользователь> на канал <канал>\n";
    help+="ISON <имена пользователей> - Запрашивает статус пользователей, перечисленных в первом аргументе команды <имена пользователей>, и разделенных пробелами.\n";
    help+="JOIN <каналы> [<ключи>] - Позволяет зайти на каналы, заданные в виде разделенного запятыми списка <каналы>. \nТакже позволяет указать пароли, если они нужны, в разделенном запятыми списке <ключи>.Если канал(ы) не существуют, они будут созданы.\n";
    help+="LIST [<каналы> [<сервер>]] - Возвращает список всех каналов на сервере.Если задан разделенный запятыми список <каналы>, возвращает их топики.\nЕсли определен <сервер>, команда передается серверу <сервер>.\n";
    help+="NAMES [<каналы>] - Возвращает список пользователей, находящихся на каналах, определённых в разделенном запятыми списке <каналы>.\nЕсли аргумент <каналы> не определен, отображает всех пользователей, распределенных по именам каналов. \n";
    help+="NICK <имя пользователя> - Позволяет пользователю изменить его ник в IRC.\n";
    help+="NOTICE <цель сообщения> <сообщение> - Эта команда работает так же, как PRIVMSG,\nза исключением того, что автоматический ответ не может быть отправлен в ответ на сообщение NOTICE.\n";
    help+="PART <каналы> - Позволяет пользователю покинуть каналы, определённые в разделенном запятыми списке <каналы>.\n";
    help+="PING <сервер> - Проверяет наличие соединения. В ответ на сообщение PING возвращается ответ PONG.\n";
    help+="PRIVMSG <цель сообщения> <сообщение>- Отправляет приватное сообщение <сообщение> для <цель сообщения>,\nкоторой может являться пользователь или канал.\n";
    help+="QUIT [<сообщение>] - Отключает пользователя от сервера.\n";
    help+="SQUIT <сервер> <комментарий> - Заставляет сервер <сервер> выйти из сети.\n";
    help+="STATS <запрос> [<сервер>]- Возвращает статистику текущего сервера, или сервера <сервер>, если он определен.\n";
    help+="WHO [<имя>] - Возвращает список пользователей, совпадающих с <имя>.\n";
    help+="WHOIS [<сервер>] <имена пользователей> - Возвращает информацию о пользователях, определённых в разделенном запятыми списке <имена пользователей>\n";
    help+="WHOWAS <имя пользователя> [<сетчик> [<сервер>]] - Возвращает информацию об имени пользователя, которое сейчас не используется\n";

    main_window->label->setToolTip(help);
}

bool Window_client::read_data()
{
    QFile file(file_name);
    Data_for_server data;
    int count_users = 0;
    int count_channels = 0;
    QString name_user;
    QString name_channel;

    if(!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this,tr("ERROR"), tr("Cannot read file.\n For work will creat new file."));
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

           stream>>count_channels;
           for(int i=0; i<count_channels; i++)
           {
               stream>>name_channel;
               add_channel(name_channel, cursor==0?true:false);
           }
           cursor++;
    }
    file.close();
    cursor = 0;
    if(main_window->servers->count()!=0)
    {
        main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::blue));
        main_window->servers->item(cursor)->setTextColor(QColor(Qt::black));
    }
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
    QSignalMapper *signal_map_three = new QSignalMapper;

    QAction *action_one;
    QAction *action_two;
    QAction *action_three;

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

    action_two = sub_menu.addAction("Change info");
    connect(action_two,SIGNAL(triggered()),signal_map_two,SLOT(map()));
    signal_map_two->setMapping(action_two,index_item);
    connect(signal_map_two,SIGNAL(mapped(int)),SLOT(slot_change_server(int)));

    action_three = sub_menu.addAction("Delete");
    connect(action_three,SIGNAL(triggered()),signal_map_three,SLOT(map()));
    signal_map_three->setMapping(action_three,index_item);
    connect(signal_map_three,SIGNAL(mapped(int)),SLOT(slot_delete_server(int)));

    sub_menu.exec(item_pos);

    delete signal_map_one;
    delete signal_map_two;
    delete signal_map_three;

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
    QSignalMapper *signal_map_two = new QSignalMapper;

    QAction *action_one;
    QAction *action_two;

    action_one = sub_menu.addAction("Change info");
    connect(action_one,SIGNAL(triggered()),signal_map_one,SLOT(map()));
    signal_map_one->setMapping(action_one,index_item);
    connect(signal_map_one,SIGNAL(mapped(int)),SLOT(slot_change_user(int)));

    action_two = sub_menu.addAction("Delete");
    connect(action_two,SIGNAL(triggered()),signal_map_two,SLOT(map()));
    signal_map_two->setMapping(action_two,index_item);
    connect(signal_map_two,SIGNAL(mapped(int)),SLOT(slot_delete_user(int)));

    sub_menu.exec(item_pos);

    delete signal_map_one;
    delete signal_map_two;
}

void Window_client::slot_sub_menu_channels(const QPoint &pos)
{
    QPoint item_pos = main_window->channels->mapToGlobal(pos);
    QMenu sub_menu;

    int index_item = main_window->channels->indexAt(pos).row();
    if(index_item==-1)
    {
        return;
    }

    QSignalMapper *signal_map_one = new QSignalMapper;
    QSignalMapper *signal_map_two = new QSignalMapper;

    QAction *action_one;
    QAction *action_two;

    action_one = sub_menu.addAction("Change info");
    connect(action_one,SIGNAL(triggered()),signal_map_one,SLOT(map()));
    signal_map_one->setMapping(action_one,index_item);
    connect(signal_map_one,SIGNAL(mapped(int)),SLOT(slot_change_channel(int)));

    action_two = sub_menu.addAction("Delete");
    connect(action_two,SIGNAL(triggered()),signal_map_two,SLOT(map()));
    signal_map_two->setMapping(action_two,index_item);
    connect(signal_map_two,SIGNAL(mapped(int)),SLOT(slot_delete_channel(int)));

    sub_menu.exec(item_pos);

    delete signal_map_one;
    delete signal_map_two;
}

void Window_client::slot_connected_to_server(int index)
{
    main_window->statusBar->clearMessage();
     if(main_window->servers->item(index)->backgroundColor()==QColor(Qt::gray))
     {
         main_window->servers->item(index)->setBackgroundColor(QColor(Qt::white));
         main_window->servers->item(index)->setTextColor(QColor(Qt::black));
     }
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

void Window_client::slot_change_info_of_server(int index)
{
    if(index<0|| index>=_sockets.count())
    {
        return;
    }

    Data_for_server info = window_for_add_server->get_info();
    window_for_add_server->close_window();
    _sockets.at(index)->change_all_info(info);
    main_window->statusBar->showMessage(QString("you need to reconnect("+info.host+")"));
    main_window->servers->item(index)->setBackgroundColor(QColor(Qt::gray));
    main_window->servers->item(index)->setTextColor(QColor(Qt::white));
    main_window->servers->item(index)->setText(info.host);
}

void Window_client::slot_change_info_of_user(int index)
{
    QString info = window_for_add_user->get_info();
    window_for_add_user->close_window();
    _sockets.at(cursor)->change_user(index,info);
    main_window->users->item(index)->setText(info);
}

void Window_client::slot_change_info_of_channel(int index)
{
    QString info = window_for_add_channel->get_info();
    window_for_add_channel->close_window();
    _sockets.at(cursor)->change_channel(index,info);
    main_window->channels->item(index)->setText(info);
}

void Window_client::slot_read_info_of_user()
{
    QString nick = window_for_add_user->get_info();
    add_user(nick);
    window_for_add_user->close_window();
}

void Window_client::slot_read_info_of_channel()
{
    QString name = window_for_add_channel->get_info();
    add_channel(name);
    window_for_add_channel->close_window();
}

void Window_client::slot_add_server()
{
    main_window->statusBar->clearMessage();
    window_for_add_server->clear_window();
    window_for_add_server->close_window();
    window_for_add_server->open_window();
}

void Window_client::slot_add_user()
{
    if(main_window->servers->count()!=0)
    {
        window_for_add_user->clear_window();
        window_for_add_user->clear_window();
        window_for_add_user->close_window();
        window_for_add_user->open_window();
    }
    else
    {
        main_window->statusBar->showMessage("Servers isn't.");
    }
}

void Window_client::slot_add_channel()
{
    if(main_window->servers->count()!=0)
    {
        window_for_add_channel->clear_window();
        window_for_add_channel->clear_window();
        window_for_add_channel->close_window();
        window_for_add_channel->open_window();
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
    _box_for_users_input.push_back(QString());
    main_window->servers->addItem(socket->get_host());

    connect(socket,SIGNAL(change_chat(IRC_Server *)),this,SLOT(slot_recv_message(IRC_Server *)));
    connect(socket,SIGNAL(error_connect(IRC_Server*)),this,SLOT(slot_error_connect(IRC_Server*)));
}

void Window_client::slot_error_connect(IRC_Server *socket)
{
    int index = find_server(socket);
    if(index!=-1)
    {
        main_window->servers->item(index)->setBackgroundColor(QColor(Qt::red));
        main_window->servers->item(index)->setTextColor(QColor(Qt::black));
    }
}

void Window_client::slot_recv_message(IRC_Server *socket)
{
    if(socket == _sockets.at(cursor))
    {
        _sockets.at(cursor)->show_chat_server(main_window->chat);
        main_window->chat->scrollToItem(main_window->chat->item(main_window->chat->count()-1));
    }
    else
    {
        int index = find_server(socket);
        if(index != -1)
        {
            main_window->servers->item(index)->setBackgroundColor(QColor(Qt::yellow));
            main_window->servers->item(index)->setTextColor(QColor(Qt::black));
        }
    }
}

int Window_client::find_server(IRC_Server *socket)
{
    QList<IRC_Server*>::Iterator it = _sockets.begin();
    for(int index = 0; it!=_sockets.end(); it++, index++)
    {
        if(*it==socket)
        {
            return index;
        }
    }
    return -1;
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

void Window_client::add_channel(QString name, bool flag_print)
{
    if(main_window->servers->count()!=0)
    {
        if(flag_print == true)
        {
             main_window->channels->addItem(name);
        }
        _sockets.at(cursor)->add_channels(name);
    }
}

void Window_client::slot_delete_server(int index)
{
    IRC_Server *item = _sockets.takeAt(index);
    delete item;
    main_window->servers->takeItem(index);
    _box_for_users_input.takeAt(index);
    if(cursor==index)
    {
        if(main_window->servers->count()==0)
        {
            main_window->users->clear();
            main_window->chat->clear();
            main_window->channels->clear();
            main_window->statusBar->clearMessage();
            cursor = 0;
        }
        else
        {
            cursor = 0;
            _sockets.at(cursor)->show_chat_server(main_window->chat);
            _sockets.at(cursor)->show_channels_server(main_window->channels);
            _sockets.at(cursor)->show_users_server(main_window->users);
            main_window->field_for_input_text->setText(_box_for_users_input.at(cursor));
            main_window->field_for_input_text->setFocus();

            main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::blue));
            main_window->servers->item(cursor)->setTextColor(QColor(Qt::white));
        }
    }
}

void Window_client::slot_delete_user(int index)
{
    _sockets.at(cursor)->delete_user(index);
    main_window->users->takeItem(index);
}

void Window_client::slot_delete_channel(int index)
{
    _sockets.at(cursor)->delete_channels(index);
    main_window->channels->takeItem(index);
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
            if(main_window->servers->item(cursor)->backgroundColor()!=QColor(Qt::gray))
            {
                main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::blue));
                main_window->servers->item(cursor)->setTextColor(QColor(Qt::white));
            }
            return;
        }


        *(_box_for_users_input.begin()+cursor) = main_window->field_for_input_text->text();
        if(main_window->servers->item(cursor)->backgroundColor()!=QColor(Qt::gray))
        {
            if(_sockets.at(cursor)->is_connected())
            {
                main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::green));
            }
            else
            {
                main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::white));
                main_window->servers->item(cursor)->setTextColor(QColor(Qt::black));
            }
        }

        cursor = index.row();

        main_window->field_for_input_text->setText(_box_for_users_input.at(cursor));

        _sockets.at(cursor)->show_chat_server(main_window->chat);
        _sockets.at(cursor)->show_users_server(main_window->users);
        _sockets.at(cursor)->show_channels_server(main_window->channels);

        main_window->chat->scrollToItem(main_window->chat->item(main_window->chat->count()-1));

        main_window->field_for_input_text->setFocus();

        if(main_window->servers->item(cursor)->backgroundColor()!=QColor(Qt::gray))
        {
            main_window->servers->item(cursor)->setBackgroundColor(QColor(Qt::blue));
            main_window->servers->item(cursor)->setTextColor(QColor(Qt::white));
        }
}

void Window_client::on_users_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    main_window->field_for_input_text->setText(QString("PRIVMSG "+main_window->users->item(index.row())->text()+" :"));
    main_window->field_for_input_text->setFocus();
}

void Window_client::on_channels_doubleClicked(const QModelIndex &index)
{
    if(!index.isValid())
    {
        return;
    }
    main_window->field_for_input_text->setText(QString("JOIN "+main_window->channels->item(index.row())->text()));
    main_window->field_for_input_text->setFocus();
}
