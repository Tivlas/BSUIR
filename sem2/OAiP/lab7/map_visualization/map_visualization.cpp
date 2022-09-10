#include "map_visualization.h"
#include "stdafx.h"

map_visualization::map_visualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
}

void map_visualization::print(const map<int, QString>& mp)
{
	ui.tableWidget->clear();
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setRowCount(mp.size());
	int i = 0;
	for (auto it = mp.cbegin(); it != mp.cend(); ++it)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(it->first)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(it->second));
		i++;
	}
}

void map_visualization::on_searchBtn_clicked()
{
	QString key = ui.searchEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	auto it = mp.find(key.toInt());
	if (it == mp.end())
	{
		QMessageBox::warning(this, "Ошибка", "Нет такого ключа");
		ui.searcResultEdit->setText("Не найдено");
		return;
	}
	ui.searcResultEdit->setText(it->second);
}

void map_visualization::on_removeBtn_clicked()
{
	if (!mp.empty())
	{
		QString key = ui.removeEdit->text();
		if (key.isEmpty())
		{
			QMessageBox::warning(this, "Ошибка", "Введите ключ");
			return;
		}
		mp.erase(key.toInt());
		print(mp);
	}
	else {
		QMessageBox::warning(this, "Ошибка", "Дерево пустое");
		return;
	}
}

void map_visualization::on_addBtn_clicked()
{
	QString key = ui.addKeyEdit->text();
	QString value = ui.addValueEdit->text();
	if (key.isEmpty() || value.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ и значение");
		return;
	}
	mp.insert({ key.toInt(), value });
	ui.addKeyEdit->clear();
	print(mp);
}

void map_visualization::on_clearBtn_clicked()
{
	mp.clear();
	ui.tableWidget->clear();
}

void map_visualization::on_operatorBtn_clicked()
{
	QString key = ui.operatorKeyEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	auto ref = mp[key.toInt()];
	ui.operatorResEdit->setText(ref);
	print(mp);
}
