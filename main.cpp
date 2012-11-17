#include <QtGui/QApplication>

#include "ui/MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    int ret = a.exec();

    return ret;
}
