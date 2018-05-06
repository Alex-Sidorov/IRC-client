#include "Window_For_Add_Server.h"

void Window_For_Add_Server::slot_for_working_info()
{
    if(port_edit->text().isEmpty()||host_edit->text().isEmpty()||real_name_edit->text().isEmpty()
                || nick_edit->text().isEmpty()|| name_edit->text().isEmpty())
        {
            error_label->setText("Error. Fill in all the fields.");
        }
        else
        {
            info.host = host_edit->text();
            info.name = name_edit->text();
            info.nick = nick_edit->text();
            info.port = port_edit->text().toInt();
            info.real_name = real_name_edit->text();
            emit this->entry_info();
        }
}

void Window_For_Add_Server::clear_window()
{
    port_edit->setText("6667");
    host_edit->clear();
    real_name_edit->clear();
    nick_edit->clear();
    name_edit->clear();
}

struct Data_for_server Window_For_Add_Server::get_info()const
{
    return info;
}

void Window_For_Add_Server::open_window()const
{
    widget->show();
}

void Window_For_Add_Server::close_window()const
{
    widget->close();
}

Window_For_Add_Server::Window_For_Add_Server()
{
    widget = new QWidget;

    main_layout = new QVBoxLayout;
    port_layout = new QVBoxLayout;
    host_layout = new QVBoxLayout;
    nick_layout = new QVBoxLayout;
    name_layout = new QVBoxLayout;
    real_name_layout = new QVBoxLayout;

    host_port_layout = new QHBoxLayout;
    nick_name_layout = new QHBoxLayout;

    top_label = new QLabel("Input info for server");
    error_label = new QLabel;

    nick_label = new QLabel("Nick");
    name_label = new QLabel("Name");
    real_name_label = new QLabel("Real name");
    port_label = new QLabel("Port");
    host_label = new QLabel("Host");

    enter = new QPushButton("Enter");

    port_edit = new QLineEdit("6667");
    host_edit = new QLineEdit;
    name_edit = new QLineEdit;
    nick_edit = new QLineEdit;
    real_name_edit = new QLineEdit;

    port_layout->addWidget(port_label);
    port_layout->addWidget(port_edit);

    host_layout->addWidget(host_label);
    host_layout->addWidget(host_edit);

    nick_layout->addWidget(nick_label);
    nick_layout->addWidget(nick_edit);

    name_layout->addWidget(name_label);
    name_layout->addWidget(name_edit);

    real_name_layout->addWidget(real_name_label);
    real_name_layout->addWidget(real_name_edit);

    host_port_layout->addLayout(host_layout);
    host_port_layout->addLayout(port_layout);

    nick_name_layout->addLayout(nick_layout);
    nick_name_layout->addLayout(name_layout);

    main_layout->addWidget(top_label);
    main_layout->addWidget(error_label);
    main_layout->addLayout(host_port_layout);
    main_layout->addLayout(nick_name_layout);
    main_layout->addLayout(real_name_layout);
    main_layout->addWidget(enter);

    widget->setLayout(main_layout);

    connect(enter,SIGNAL(clicked()),this,SLOT(slot_for_working_info()));
}


Window_For_Add_Server::~Window_For_Add_Server()
{
}
