#include "mystringvisualization.h"
#include "stdafx.h"

MyStringVisualization::MyStringVisualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void MyStringVisualization::print(const MyString& string)
{
	if (string.size() != 0)
	{
		QString res = "";
		for (int i = 0; i < string.size(); i++)
		{
			res += string[i];
		}
		ui.textEdit->setText(res);
	}
	else
	{
		ui.textEdit->setText("");
	}
	ui.sizeLable->setText(QString::number(string.size()));
	ui.capacityLable->setText(QString::number(string.capacity()));
}

void MyStringVisualization::on_pop_backBtn_clicked()
{
	str.pop_back();
	print(str);
}

void MyStringVisualization::on_push_backBtn_clicked()
{
	QChar value = ui.push_backValueEdit->text().back();
	char val = value.toLatin1();
	str.push_back(val);
	print(str);
}

void MyStringVisualization::on_resizeBtn_clicked()
{
	QString value = ui.resizeValueEdit->text();
	if (value == "") {
		QMessageBox::warning(this, "Warning", "Please enter a value");
		return;
	}
	str.resize(ui.resizeSizeEdit->text().toInt(), value.back().toLatin1());
	print(str);
}

void MyStringVisualization::on_reserveBtn_clicked()
{
	str.reserve(ui.reserveSizeEdit->text().toInt());
	print(str);
}

void MyStringVisualization::on_insertBtn_clicked()
{
	QString pos = ui.insertPosEdit->text();
	QString value = ui.insertValueEdit->text();
	QString count = ui.insertCountEdit->text();
	if (count == "") {
		str.insert(str.begin() + pos.toInt(), value.back().toLatin1());
	}
	else {
		str.insert(str.begin() + pos.toInt(), count.toInt(), value.back().toLatin1());
	}
	print(str);
}

void MyStringVisualization::on_eraseBtn_clicked()
{
	QString first = ui.eraseFirstEdit->text();
	QString second = ui.eraseSecondEdit->text();
	if (second == "") {
		str.erase(str.begin() + first.toInt());
	}
	else {
		str.erase(str.begin() + first.toInt(), str.begin() + second.toInt());
	}
	print(str);
}

void MyStringVisualization::on_appendBtn_clicked()
{
	QChar value = ui.appendValueEdit->text().back();
	QString count = ui.appendCountEdit->text();
	str.append(count.toInt(), value.toLatin1());
	print(str);
}

void MyStringVisualization::on_quitBtn_clicked()
{
	QApplication::quit();
}

void MyStringVisualization::on_clearBtn_clicked()
{
	str.clear();
	print(str);
}
