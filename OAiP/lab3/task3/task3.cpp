#include "task3.h"
#include "stdafx.h"
#include "Stack.h"

task3::task3(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	int n = 7;
	
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Выражение" << "a" << "b" << "c" << "d" << "e" << "Результат");
	ui.tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "13" << "14" << "15");
	
}

int task3::precedence(QChar op)
{
    if (op == '(') {
        return 0;
    }
    else if (op == '+' || op == '-') {
        return 1;
    }
    else if (op == '*' || op == '/') {
        return 2;
    }
    return 3;
}
