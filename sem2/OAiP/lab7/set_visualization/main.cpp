#include "set_visualization.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    set_visualization w;
    w.show();
    return a.exec();
}
