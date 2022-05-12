#include "task1.h"
#include "stdafx.h"

task1::task1(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
	ui.searchEdit->setPlaceholderText("Ключ");
	ui.removeEdit->setPlaceholderText("Ключ");
	ui.addKeyEdit->setPlaceholderText("Ключ");
	ui.addValueEdit->setPlaceholderText("Значение");
}

void task1::print(const std::vector<std::pair<int, QString>>& v)
{
	ui.tableWidget->clear();
	ui.tableWidget->setColumnCount(2);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
	ui.tableWidget->setRowCount(v.size());
	for (int i = 0; i < v.size(); i++) {
		ui.tableWidget->setItem(i, 0, new QTableWidgetItem(QString::number(v[i].first)));
		ui.tableWidget->setItem(i, 1, new QTableWidgetItem(v[i].second));
	}
}

void task1::print_keys_order(const std::vector<int>& v)
{
	ui.plainTextEdit->clear();
	QString order;
	for (int i = 0; i < v.size(); i++) {
		order += QString::number(v[i]) + " -> ";
	}
	order.erase(order.end() - 4, order.end());
	ui.plainTextEdit->setPlainText("Порядок добавления вершин(по ключу):");
	ui.plainTextEdit->appendPlainText(order);
}

void task1::on_addBtn_clicked()
{
	QString key = ui.addKeyEdit->text();
	QString value = ui.addValueEdit->text();
	if (key.isEmpty() || value.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ и значение");
		return;
	}
	tree.insert(key.toInt(), value);
	pushed_keys_order.push_back(key.toInt());
	print_keys_order(pushed_keys_order);
	ui.addKeyEdit->clear();
	on_comboBox_currentTextChanged(ui.comboBox->currentText());
}

void task1::on_comboBox_currentTextChanged(const QString& cur_text)
{
	if (cur_text == "Прямой") {
		tree.traverse_preorder(v);
	}
	else if (cur_text == "Обратный") {
		tree.traverse_postorder(v);
	}
	else if (cur_text == "Симметричный") {
		tree.traverse_inorder(v);
	}
	if (v.size())
	{
		print(v);
	}
	else {
		ui.tableWidget->clear();
		ui.tableWidget->setColumnCount(2);
		ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Ключ" << "Значение");
		QMessageBox::warning(this, "Внимание", "Дерево пустое");
		ui.plainTextEdit->clear();
	}
	v.clear();
}

void task1::on_removeBtn_clicked()
{
	QString key = ui.removeEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	tree.remove(key.toInt());
	on_comboBox_currentTextChanged(ui.comboBox->currentText());
}

void task1::on_searchBtn_clicked()
{
	QString key = ui.searchEdit->text();
	if (key.isEmpty())
	{
		QMessageBox::warning(this, "Ошибка", "Введите ключ");
		return;
	}
	QString value = "";
	try
	{
		value = tree.search(key.toInt());
	}
	catch (const std::exception& ex)
	{
		QMessageBox::warning(this, "Ошибка", ex.what());
		return;
	}
	ui.searchLbl->setText(value);
}

