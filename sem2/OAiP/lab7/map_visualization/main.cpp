#include "map_visualization.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    map_visualization w;
    w.show();
    return a.exec();
}
