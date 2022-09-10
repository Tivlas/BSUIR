#include "u_map_visualization.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    U_map_Visualization w;
    w.show();
    return a.exec();
}
