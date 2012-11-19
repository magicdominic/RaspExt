#include <QtGui/QApplication>

#include "ui/MainWindow.h"

#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if( getuid() != 0)
    {
        printf("This application needs root privileges, it seems that you are not root. Some functionality might not be available\n");
    }

    QApplication a(argc, argv);
    MainWindow w;

    w.show();

    int ret = a.exec();

    return ret;
}
