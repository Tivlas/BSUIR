#include "bitset_visualization.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    BitSet_Visualization w;
    w.show();
    return a.exec();
}
