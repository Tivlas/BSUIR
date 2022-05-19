#include "set_visualization.h"
#include "stdafx.h"

set_visualization::set_visualization(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

void set_visualization::print(const set<int, char>& st)
{
	ui.tableWidget->clear();
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setRowCount(st.size());
	int i = 0;
	for (auto it = st.crbegin(); it != st.crend(); ++it)
	{
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(it->first)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(it->second));
		i++;
	}
}

void set_visualization::on_searchBtn_clicked()
{
	QString key = ui.searchEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	auto it = st.find(key.toInt());
	if (it == st.end())
	{
		QMessageBox::warning(this, "Ошибка", "Нет такого ключа");
		ui.searcResultEdit->setText("Не найдено");
		return;
	}
	ui.searcResultEdit->setText(QString::number(it->first));
}

void set_visualization::on_removeBtn_clicked()
{
	if (!st.empty())
	{
		QString key = ui.removeEdit->text();
		if (key.isEmpty())
		{
			QMessageBox::warning(this, "Ошибка", "Введите ключ");
			return;
		}
		st.erase(key.toInt());
		print(st);
	}
	else {
		QMessageBox::warning(this, "Ошибка", "Дерево пустое");
		return;
	}
}

void set_visualization::on_addBtn_clicked()
{
	QString key = ui.addKeyEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	st.insert(key.toInt());
	ui.addKeyEdit->clear();
	print(st);
}

void set_visualization::on_clearBtn_clicked()
{
	st.clear();
	ui.tableWidget->clear();
}

