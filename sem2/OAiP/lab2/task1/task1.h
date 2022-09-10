#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task1.h"
#include "Stack.h"
class task1 : public QMainWindow
{
	Q_OBJECT

public:
	task1(QWidget* parent = Q_NULLPTR);

public slots:
	void on_pushBtn_clicked();
	void on_popBtn_clicked();
	void on_quitBtn_clicked();
private:
	Ui::task1Class ui;
	Stack<int> s;
	size_t numberOfRows = 0;
};
