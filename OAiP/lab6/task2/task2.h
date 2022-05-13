#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task2.h"

class task2 : public QMainWindow
{
    Q_OBJECT

public:
    task2(QWidget *parent = Q_NULLPTR);

private:
    Ui::task2Class ui;
};
