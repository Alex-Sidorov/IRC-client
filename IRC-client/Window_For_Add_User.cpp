#include "Window_For_Add_User.h"

void Window_For_Add_User::slot_for_working_info()
{
    if(nick_edit->text().isEmpty())
    {
        error_label->setText("Error. Input info.");
    }
    else
    {
        nick=nick_edit->text();
        emit entry_info();
    }
}

void Window_For_Add_User::clear_window()
{
    error_label->clear();
    nick_edit->clear();
}

void Window_For_Add_User::open_window()
{
    widget->show();
}

void Window_For_Add_User::close_window()
{
    widget->close();
}

QString Window_For_Add_User::get_info()const
{
    return nick;
}

Window_For_Add_User::Window_For_Add_User()
{

    main_layout = new QVBoxLayout;

    enter = new QPushButton("Enter");

    widget = new QWidget;

    top_label = new QLabel("Input nick of user");

    error_label = new QLabel;

    nick_label = new QLabel("Nick");

    nick_edit = new QLineEdit;

    main_layout->addWidget(top_label,0,Qt::AlignHCenter);
    main_layout->addWidget(error_label,0,Qt::AlignHCenter);
    main_layout->addWidget(nick_label,0,Qt::AlignHCenter);
    main_layout->addWidget(nick_edit);
    main_layout->addWidget(enter);

    widget->setLayout(main_layout);

    connect(enter,SIGNAL(clicked()),SLOT(slot_for_working_info()));
    connect(nick_edit,SIGNAL(returnPressed()),this,SLOT(slot_for_working_info()));
}

Window_For_Add_User::~Window_For_Add_User()
{
}
