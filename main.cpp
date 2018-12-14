#include "widget.h"
#include "client.h"

#include <QApplication>
#include <QFile>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Widget w;
    w.show();

    Client c1;
    c1.show();

    Client c3;
    c3.show();

    return a.exec();
}
