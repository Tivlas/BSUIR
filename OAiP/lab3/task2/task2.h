#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task2.h"
#include "Stack.h"

class task2 : public QMainWindow
{
    Q_OBJECT

public:
    task2(QWidget *parent = Q_NULLPTR);
    void checkText(const QString& text);
public slots:
    void on_quitBtn_clicked();
    void on_readBtn_clicked();
	void on_checkBtn_clicked();
	
private:
    Ui::task2Class ui;
    QString fileName;
    QString text;
    bool haveReadFromFile = false;
    Stack stack;
};
