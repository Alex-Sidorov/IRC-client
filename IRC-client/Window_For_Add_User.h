#ifndef WINDOW_FOR_ADD_USER_H
#define WINDOW_FOR_ADD_USER_H

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Window_For_Add_User:public QObject
{
    Q_OBJECT

public:

    QString get_info()const;

    void open_window();
    void close_window();

    void fill_form(QString nick,int index);

    void clear_window();

    Window_For_Add_User();
    virtual ~Window_For_Add_User();

signals:
    void change_info(int);
    void entry_info();

private slots:
    void slot_for_working_info();

private:

    QWidget *widget;

    QVBoxLayout *main_layout;

    QLabel *top_label;
    QLabel *error_label;

    QLabel *nick_label;
    QLineEdit *nick_edit;

    QPushButton *enter;
    QString _nick;

    int index_user;

};

#endif // WINDOW_FOR_ADD_USER_H
