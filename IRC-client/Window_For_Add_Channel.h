#ifndef WINDOW_FOR_ADD_CHANNEL_H
#define WINDOW_FOR_ADD_CHANNEL_H

#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class Window_For_Add_Channel:public QObject
{
    Q_OBJECT

public:

    QString get_info()const;

    void open_window();
    void close_window();

    void clear_window();
    void fill_form(QString name, int index);

    Window_For_Add_Channel();
    virtual ~Window_For_Add_Channel();


signals:
    void entry_info();
    void change_info(int);

private slots:
    void slot_for_working_info();

private:

    QWidget *widget;

    QVBoxLayout *main_layout;

    QLabel *top_label;
    QLabel *error_label;

    QLabel *name_label;
    QLineEdit *name_edit;

    QPushButton *enter;
    QString _name;

    int index_channel;
};

#endif // WINDOW_FOR_ADD_CHANNEL_H
