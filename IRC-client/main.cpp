#include "Window_Client.h"
#include "IRC_Server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window_client window;
    if(!window.read_data())
    {
        return -1;
    }

    window.show();

    return a.exec();
}
