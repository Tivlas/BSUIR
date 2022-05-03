#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mystringvisualization.h"
#include "MyString.h"

class MyStringVisualization : public QMainWindow
{
    Q_OBJECT

public:
    MyStringVisualization(QWidget *parent = Q_NULLPTR);
    void print(const MyString& string);
private slots:
    void on_clearBtn_clicked();
    void on_pop_backBtn_clicked();
    void on_push_backBtn_clicked();
    void on_resizeBtn_clicked();
    void on_reserveBtn_clicked();
    void on_insertBtn_clicked();
    void on_eraseBtn_clicked();
    void on_appendBtn_clicked();
    void on_quitBtn_clicked();

private:
    Ui::MyStringVisualizationClass ui;
    MyString str;
};
