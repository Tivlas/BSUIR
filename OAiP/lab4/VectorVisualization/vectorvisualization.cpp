#pragma once
#include "vectorvisualization.h"
#include "stdafx.h"
#include <regex>

VectorVisualization::VectorVisualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	notInBold = ui.textEdit->currentCharFormat();
	inBold.setFontWeight(QFont::Bold);
}

void VectorVisualization::print(Vector<int>& vector)
{
	ui.textEdit->clear();
	ui.sizeLable->setText(QString::number(vector.size()));
	ui.capacityLable->setText(QString::number(vector.capacity()));
	QString vectStr = "";
	for (int i = 0; i < vector.size(); i++)
	{
		vectStr += QString::number(vector[i]) + " ";
	}
	ui.textEdit->setCurrentCharFormat(inBold);
	ui.textEdit->append("Vector:\n");
	ui.textEdit->setCurrentCharFormat(notInBold);
	ui.textEdit->append(vectStr);
}

void VectorVisualization::on_pop_backBtn_clicked()
{
	vector.pop_back();
	print(vector);
}

void VectorVisualization::on_push_backBtn_clicked()
{
	vector.push_back(ui.push_backValueEdit->text().toInt());
	print(vector);
}

void VectorVisualization::on_emplace_backBtn_clicked()
{
	vector.emplace_back(ui.emplace_backValueEdit->text().toInt());
	print(vector);
}

void VectorVisualization::on_emplaceBtn_clicked()
{
	vector.emplace(vector.cbegin() + ui.emplacePosEdit->text().toInt(), ui.emplaceValueEdit->text().toInt());
	print(vector);
}

void VectorVisualization::on_resizeBtn_clicked()
{
	QString value = ui.resizeValueEdit->text();
	if (value.isEmpty())
	{
		vector.resize(ui.resizeSizeEdit->text().toInt());
	}
	else
	{
		vector.resize(ui.resizeSizeEdit->text().toInt(), value.toInt());
	}
	print(vector);
}

void VectorVisualization::on_reserveBtn_clicked()
{
	vector.reserve(ui.reserveSizeEdit->text().toInt());
	print(vector);
}

void VectorVisualization::on_assignBtn_clicked()
{
	vector.assign(ui.assignSizeEdit->text().toInt(), ui.assignValueEdit->text().toInt());
	print(vector);
}

void VectorVisualization::on_insertBtn_clicked()
{
	QString pos = ui.insertPosEdit->text();
	QString value = ui.insertValueEdit->text();
	QString count = ui.insertCountEdit->text();
	if (count == "") {
		vector.insert(vector.cbegin() + pos.toInt(), value.toInt());
	}
	else {
		vector.insert(vector.cbegin() + pos.toInt(), count.toInt(), value.toInt());
	}
	print(vector);
}

void VectorVisualization::on_eraseBtn_clicked()
{
	QString first = ui.eraseFirstEdit->text();
	QString second = ui.eraseSecondEdit->text();
	if (second == "") {
		vector.erase(vector.cbegin() + first.toInt());
	}
	else {
		vector.erase(vector.begin() + first.toInt(), vector.begin() + second.toInt());
	}
	print(vector);
}

void VectorVisualization::on_clearBtn_clicked() {
	vector.clear();
	print(vector);
}

void VectorVisualization::on_quitBtn_clicked()
{
	QApplication::quit();
}