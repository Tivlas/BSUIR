#include "queuevisualization.h"
#include "stdafx.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QueueVisualization w;
    w.show();
    return a.exec();
}
