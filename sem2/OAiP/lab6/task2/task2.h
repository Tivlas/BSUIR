#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task2.h"
#include "HashTable.h"

class task2 : public QMainWindow
{
    Q_OBJECT

public:
    task2(QWidget *parent = Q_NULLPTR);
	void print(const HashTable& table);
private slots:
	void on_findBtn_clicked();
	void on_insertBtn_clicked();
	void on_removeBtn_clicked();

private:
    Ui::task2Class ui;
	HashTable hash_table;
};
