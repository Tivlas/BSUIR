#pragma once
#include "task1.h"
#include "stdafx.h"

task1::task1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.table->setColumnCount(1);
	ui.table->setHorizontalHeaderLabels(QStringList() << "Number");
}
void task1::on_pushBtn_clicked()
{
	s.push(rand() % 100);
	QTableWidgetItem* item = new QTableWidgetItem(QString::number(s.topValue()));
	ui.table->insertRow(numberOfRows);
	ui.table->setItem(numberOfRows, 0, item);
	++numberOfRows;
}

void task1::on_popBtn_clicked()
{
	s.pop();
	if (!numberOfRows) return;
	ui.table->removeRow(--numberOfRows);
}

void task1::on_quitBtn_clicked()
{
	QApplication::quit();
}
