#pragma once

#include <QtWidgets/QWidget>
#include "ui_lab1.h"
#include "Rect.h"
#include <QTimer>



class LAB1 : public QWidget
{
    Q_OBJECT

public:
    LAB1(QWidget *parent = Q_NULLPTR);

private slots:
    void on_Start_clicked();
    void on_turnOn_clicked();
    void moving();
    void turnOnLights();

protected:
    void paintEvent(QPaintEvent* e);

private:
    Ui::LAB1Class ui;
    
    Bus* bus = new Bus(450, 300);
    QTimer* timer = new QTimer();
   
};
