#include "dequevisualization.h"
#include "stdafx.h"

DequeVisualization::DequeVisualization(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void DequeVisualization::print(Deque<int>& deq)
{
	ui.textEdit->clear();
	ui.sizeLable->setText(QString::number(deq.size()));
	QString res = "";
	for(auto it = deq.begin(); it != deq.end(); it++)
	{
		res += QString::number(*it) + " ";
	}
	ui.textEdit->setText(res);
}

void DequeVisualization::on_insertBtn_clicked()
{
	QString pos = ui.insertPosEdit->text();
	QString value = ui.insertValueEdit->text();
	QString count = ui.insertCountEdit->text();
	if (count == "") {
		deq.insert(deq.begin() + pos.toInt(), value.toInt());
	}
	else {
		deq.insert(deq.begin() + pos.toInt(), count.toInt(), value.toInt());
	}
	print(deq);
}

void DequeVisualization::on_eraseBtn_clicked()
{
	QString first = ui.eraseFirstEdit->text();
	QString second = ui.eraseSecondEdit->text();
	if (second == "") {
		deq.erase(deq.begin() + first.toInt());
	}
	else {
		deq.erase(deq.begin() + first.toInt(), deq.begin() + second.toInt());
	}
	print(deq);
}

void DequeVisualization::on_push_backBtn_clicked()
{
	deq.push_back(ui.push_backValueEdit->text().toInt());
	print(deq);
}

void DequeVisualization::on_push_frontBtn_clicked()
{
	deq.push_front(ui.push_frontValueEdit->text().toInt());
	print(deq);
}

void DequeVisualization::on_clearBtn_clicked()
{
	deq.clear();
	print(deq);
}

void DequeVisualization::on_pop_backBtn_clicked()
{
	deq.pop_back();
	print(deq);
}

void DequeVisualization::on_pop_frontBtn_clicked()
{
	deq.pop_front();
	print(deq);
}

void DequeVisualization::on_resizeBtn_clicked()
{
	QString value = ui.resizeValueEdit->text();
	if (value.isEmpty())
	{
		deq.resize(ui.resizeSizeEdit->text().toInt());
	}
	else
	{
		deq.resize(ui.resizeSizeEdit->text().toInt(), value.toInt());
	}
	print(deq);
}

void DequeVisualization::on_quitBtn_clicked()
{
	QApplication::quit();
}
