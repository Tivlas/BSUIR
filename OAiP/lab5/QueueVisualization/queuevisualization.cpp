#include "queuevisualization.h"
#include "stdafx.h"

QueueVisualization::QueueVisualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	notInBold = ui.textEdit->currentCharFormat();
	inBold.setFontWeight(QFont::Bold);
}

void QueueVisualization::printInfo()
{
	ui.textEdit->clear();
	QString info = "";
	info += "";
	ui.textEdit->setCurrentCharFormat(inBold);
	ui.textEdit->append("Queue size: " + QString::number(q.size()));
	ui.textEdit->append("Pushed values:");
	ui.textEdit->setCurrentCharFormat(notInBold);
	for (auto it = pushed_values.begin(), end = pushed_values.end(); it != end; ++it)
	{
		info += QString::number(*it) + " ";
	}
	ui.textEdit->append(info);
	ui.textEdit->setCurrentCharFormat(inBold);
	if (q.size() != 0)
	{
		ui.textEdit->append("Front: " + QString::number(q.front()) + ", Back: " + QString::number(q.back()));
	}
}

void QueueVisualization::on_pushBtn_clicked()
{
	QString str = ui.lineEdit->text();
	q.push(str.toInt());
	pushed_values.push_back(str.toInt());
	printInfo();
	ui.lineEdit->clear();
}

void QueueVisualization::on_popBtn_clicked()
{
	q.pop();
	if (q.size() == 0) {
		pushed_values.clear();
	}
	printInfo();
}
