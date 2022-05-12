#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task1.h"
#include "AVL_Tree.h"
#include <vector>

class task1 : public QMainWindow
{
	Q_OBJECT

public:
	task1(QWidget* parent = Q_NULLPTR);
	void print(const std::vector<std::pair<int, QString>>& v);
	void print_keys_order(const std::vector<int>& v);

private slots:
	void on_removeBtn_clicked();
	void on_searchBtn_clicked();
	void on_addBtn_clicked();
	void on_comboBox_currentTextChanged(const QString& s);
private:
	Ui::task1Class ui;
	AVL_Tree<QString> tree;
	std::vector <std::pair<int, QString>> v;
	std::vector<int> pushed_keys_order;
};
