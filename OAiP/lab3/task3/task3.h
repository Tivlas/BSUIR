#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task3.h"
#include "Stack.h"

class task3 : public QMainWindow
{
    Q_OBJECT

public:
    task3(QWidget *parent = Q_NULLPTR);
    int precedence(QChar op);

private:
    Ui::task3Class ui;
    Stack<QChar> opStack; // operation stack
	Stack<int> numStack; // number stack
    QString postfix = "";
};
