#include "IRC_Server.h"

bool IRC_Server::connect_to_server()
{
    _socket->connectToHost(_host,_port);

    if(!_socket->isOpen())
    {
        return false;
    }

    send_message(QString("NICK " + _nick));
    send_message(QString("USER " + _name + " 8 * :" + _real_name));
    return true;
}

void IRC_Server::disconnect_to_server()
{
    _socket->disconnectFromHost();
}

void IRC_Server::slot_connected()
{
    show_message(QString("["+get_time()+"] "+_host+":connected."));
}

void IRC_Server::slot_error_connected()
{
   show_message(QString("["+get_time()+"] "+_host+":error connected."));
}

void IRC_Server::slot_recv()//TODO
{
    QString message;

    while(_socket->canReadLine())
    {
        message.append(_socket->readLine());
    }
    message.resize(message.size()-2);//delete '\r' and '\n'
    show_message(QString("["+get_time()+"] "+_host+":"+message));

}

void IRC_Server::show_message(QString message)
{
     _chat.addItem(message);
     emit change_chat();
}

void IRC_Server::send_message(QString message)
{
    if(_socket->isValid())
    {
        _socket->write((message+"\n").toUtf8());
    }
}

void IRC_Server::show_chat_server(QListWidget *chat)
{
    chat->clear();

    for(int i=0; i<_chat.count(); i++)
    {
        chat->addItem(_chat.item(i)->text());
    }
}

void IRC_Server::show_users_server(QListWidget *users)
{
    users->clear();

    for(int i=0; i<_users.count(); i++)
    {
        users->addItem(_users.item(i)->text());
    }

}

void IRC_Server::change_all_info(Data_for_server info)
{
    _name=info.name;
    _nick=info.nick;
    _host=info.host;
    _port=info.port;
    _real_name=info.real_name;
}

QString IRC_Server::get_time()
{
    QDateTime date_and_time=QDateTime::currentDateTime();
    return date_and_time.time().toString();
}

int IRC_Server::get_port()const
{
    return _port;
}

QString IRC_Server::get_host()const
{
    return _host;
}

bool IRC_Server::is_connected()const
{
    return _socket->isValid();
}

void IRC_Server::set_name(QString &name)
{
    _name = name;
}

void IRC_Server::set_real_name(QString &real_name)
{
    _real_name = real_name;
}

void IRC_Server::set_host(QString &host)
{
    _host = host;
}

void IRC_Server::set_nick(QString &nick)
{
    _nick = nick;
}

void IRC_Server::set_port(int port)
{
    _port = port;
}

void IRC_Server::delete_user(int index)
{
    if(index>0 && index<_users.sizeHintForColumn(0))
    {
        _users.removeItemWidget(_users.item(index));
    }
}

void IRC_Server::add_user(QString nick)
{
    _users.addItem(nick);
}

IRC_Server::IRC_Server(Data_for_server info):IRC_Server()
{
    _name=info.name;
    _nick=info.nick;
    _host=info.host;
    _port=info.port;
    _real_name=info.real_name;
}

IRC_Server::IRC_Server(const QString &name,const QString &real_name, const QString &nick,const QString &host,const int port=6667):IRC_Server()
{
    _name=name;
    _nick=nick;
    _host=host;
    _port=port;
    _real_name=real_name;
}

IRC_Server::IRC_Server()
{
    _socket=new QTcpSocket();
    connect(_socket,SIGNAL(connected()),SLOT(slot_connected()));
    connect(_socket,SIGNAL(error(QAbstractSocket::SocketError)),SLOT(slot_error_connected()));
    connect(_socket,SIGNAL(readyRead()),SLOT(slot_recv()));
}

IRC_Server::~IRC_Server()
{
    if(_socket->isOpen())
    {
        _socket->disconnectFromHost();
    }
    delete _socket;
}
