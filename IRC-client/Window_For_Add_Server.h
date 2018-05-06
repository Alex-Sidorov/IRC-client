#ifndef WINDOW_FOR_ADD_SERVER_H
#define WINDOW_FOR_ADD_SERVER_H

#include <QBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QLineEdit>

#include "Data_for_server.h"


class Window_For_Add_Server : public QObject
{
    Q_OBJECT

public:
    struct Data_for_server get_info()const;

    void open_window()const;
    void close_window()const;

    void clear_window();

    Window_For_Add_Server();
    virtual~Window_For_Add_Server();

public slots:
    void slot_for_working_info();

signals:
    void entry_info();

private:
    QWidget *widget;

    QVBoxLayout *main_layout;
    QVBoxLayout *port_layout;
    QVBoxLayout *host_layout;
    QVBoxLayout *nick_layout;
    QVBoxLayout *name_layout;
    QVBoxLayout *real_name_layout;

    QHBoxLayout *host_port_layout;
    QHBoxLayout *nick_name_layout;

    QLabel *top_label;
    QLabel *error_label;

    QLabel *nick_label;
    QLabel *name_label;
    QLabel *real_name_label;
    QLabel *port_label;
    QLabel *host_label;

    QPushButton *enter;

    QLineEdit *port_edit;
    QLineEdit *host_edit;
    QLineEdit *name_edit;
    QLineEdit *nick_edit;
    QLineEdit *real_name_edit;

    Data_for_server info;
};

#endif // WINDOW_FOR_ADD_SERVER_H
