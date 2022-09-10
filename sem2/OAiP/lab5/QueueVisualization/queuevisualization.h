#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_queuevisualization.h"
#include <Queue.h>
#include <Vector.h>

class QueueVisualization : public QMainWindow
{
    Q_OBJECT

public:
    QueueVisualization(QWidget *parent = Q_NULLPTR);
    void printInfo();
private slots:
	void on_popBtn_clicked();
    void on_pushBtn_clicked();

private:
    Ui::QueueVisualizationClass ui;
    Vector<int> pushed_values;
    Queue<int> q;
    QTextCharFormat notInBold, inBold;
};
