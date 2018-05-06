#include "Window_Client.h"
#include "IRC_Server.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Window_client window;

    window.show();

    return a.exec();
}
