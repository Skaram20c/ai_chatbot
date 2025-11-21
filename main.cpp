#include "MainWindow.h"
//#include "ChatWindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //ChatWindow *chat = new ChatWindow();
    //chat->show();

    MainWindow w;
    w.show();
    return app.exec();
}
