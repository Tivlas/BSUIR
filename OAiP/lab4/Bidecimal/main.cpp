#include "bidecimal.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Bidecimal w;
    w.show();
    return a.exec();
}
