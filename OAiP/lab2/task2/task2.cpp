#include "task2.h"
#include "stdafx.h"

task2::task2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.table->setColumnCount(1);
	ui.table->setHorizontalHeaderLabels(QStringList() << "Number");
}

void task2::on_push_backBtn_clicked()
{
	int valueToPush = rand() % 100;
	list.push_back(valueToPush);
	QTableWidgetItem* item = new QTableWidgetItem(QString::number(valueToPush));
	ui.table->insertRow(numberOfRows);
	ui.table->setItem(numberOfRows, 0, item);
	++numberOfRows;
}

void task2::on_pop_frontBtn_clicked()
{
	if (numberOfRows == 0) return;
	list.pop_front();
	ui.table->removeRow(0);
	--numberOfRows;
}

void task2::on_pop_backBtn_clicked()
{
	if (numberOfRows == 0) return;
	list.pop_back();
	ui.table->removeRow(--numberOfRows);
}

void task2::on_quitBtn_clicked()
{
	QApplication::quit();
}

void task2::on_push_frontBtn_clicked()
{
	int valueToPush = rand() % 100;
	list.push_front(valueToPush);
	QTableWidgetItem* item = new QTableWidgetItem(QString::number(valueToPush));
	ui.table->insertRow(0);
	ui.table->setItem(0, 0, item);
	++numberOfRows;
}

