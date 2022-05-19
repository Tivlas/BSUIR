#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_set_visualization.h"
#include "set.h"

class set_visualization : public QMainWindow
{
	Q_OBJECT

public:
	set_visualization(QWidget* parent = Q_NULLPTR);
	void print(const set<int, char>& st);
public slots:
	void on_searchBtn_clicked();
	void on_removeBtn_clicked();
	void on_addBtn_clicked();
	void on_clearBtn_clicked();
private:
	Ui::set_visualizationClass ui;
	set<int, char> st;
};
