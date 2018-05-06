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

    void open_window()const;
    void close_window()const;

    void clear_window();

    Window_For_Add_User();
    virtual ~Window_For_Add_User();

public slots:
    void slot_for_working_info();

signals:
    void entry_info();
    void closed_window();

private:

    QWidget *widget;

    QVBoxLayout *main_layout;

    QLabel *top_label;
    QLabel *error_label;

    QLabel *nick_label;
    QLineEdit *nick_edit;

    QPushButton *enter;
    QString nick;

};

#endif // WINDOW_FOR_ADD_USER_H
