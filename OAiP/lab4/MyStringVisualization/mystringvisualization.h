#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mystringvisualization.h"
#include "MyString.h"

class MyStringVisualization : public QMainWindow
{
    Q_OBJECT

public:
    MyStringVisualization(QWidget *parent = Q_NULLPTR);

private:
    Ui::MyStringVisualizationClass ui;
};
