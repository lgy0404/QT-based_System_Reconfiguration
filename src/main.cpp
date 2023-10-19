#include "widget.h"
#include "login1.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.show();


//    login1 logwindow;
//    logwindow.show();

    return a.exec();
}
