#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task.h"
#include "IntList.h"


class task : public QMainWindow
{
    Q_OBJECT

public:
    task(QWidget *parent = Q_NULLPTR);
private slots:
    void on_pushBtn_clicked();
    void on_printBtn_clicked();
    void on_clearBtn_clicked();
private:
    Ui::taskClass ui;
    IntList list;
    QTextCharFormat notInBold, inBold;
};
