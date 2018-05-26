#include "Window_For_Add_Channel.h"

void Window_For_Add_Channel::slot_for_working_info()
{
    if(name_edit->text().isEmpty())
    {
        error_label->setText("Error. Input info.");
    }
    else
    {
        if(index_channel == -1)
        {
            _name=name_edit->text();
            emit entry_info();
        }
        else
        {
            _name=name_edit->text();
            emit change_info(index_channel);
        }
    }
}

void Window_For_Add_Channel::clear_window()
{
    index_channel = -1;
    error_label->clear();
    name_edit->clear();
}

void Window_For_Add_Channel::open_window()
{
    widget->show();
}

void Window_For_Add_Channel::close_window()
{
    widget->close();
}

QString Window_For_Add_Channel::get_info()const
{
    return _name;
}

void Window_For_Add_Channel::fill_form(QString name, int index)
{
    index_channel = index;
    name_edit->setText(name);
}

Window_For_Add_Channel::Window_For_Add_Channel()
{

    main_layout = new QVBoxLayout;

    enter = new QPushButton("Enter");

    widget = new QWidget;

    top_label = new QLabel("Input name of channel");

    error_label = new QLabel;

    name_label = new QLabel("Name");

    name_edit = new QLineEdit;

    main_layout->addWidget(top_label,0,Qt::AlignHCenter);
    main_layout->addWidget(error_label,0,Qt::AlignHCenter);
    main_layout->addWidget(name_label,0,Qt::AlignHCenter);
    main_layout->addWidget(name_edit);
    main_layout->addWidget(enter);

    widget->setLayout(main_layout);

    connect(enter,SIGNAL(clicked()),SLOT(slot_for_working_info()));
    connect(name_edit,SIGNAL(returnPressed()),this,SLOT(slot_for_working_info()));
    index_channel = -1;
}

Window_For_Add_Channel::~Window_For_Add_Channel()
{
}

