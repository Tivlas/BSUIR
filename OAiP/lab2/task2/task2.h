#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task2.h"
#include "List.h"
class task2 : public QMainWindow
{
	Q_OBJECT

public:
	task2(QWidget* parent = Q_NULLPTR);
public slots:
	void on_push_frontBtn_clicked();
	void on_push_backBtn_clicked();
	void on_pop_frontBtn_clicked();
	void on_pop_backBtn_clicked();
	void on_quitBtn_clicked();


private:
	Ui::task2Class ui;
	List<int> list;
	size_t numberOfRows = 0;
};
