#include "u_map_visualization.h"
#include "stdafx.h"

U_map_Visualization::U_map_Visualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void U_map_Visualization::print(const Umap& umap)
{
	ui.textEdit->clear();
	QString str = "";
	for (size_t i = 0; i < u_map.bucket_count(); ++i) {
		str += "Bucket #" + QString::number(i) + ":   ";
		bool empty = u_map.begin(i) == u_map.end(i);
		if (empty) {
			str += "empty\n";
			continue;
		}
		for (auto it = u_map.begin(i); it != u_map.end(i); ++it) {
			str += QString::number((*it)->first) + "(" + (*it)->second + ")" + " -> ";
		}
		str.remove(str.size() - 4, 4);
		str += "\n";
	}
	ui.textEdit->setText(str);
}

void U_map_Visualization::on_insertBtn_clicked()
{
	QString key = ui.insertKeyEdit->text();
	QString value = ui.insertValueEdit->text();
	if (key.isEmpty() || value.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key and value!");
		return;
	}
	u_map.insert(iQsPair(key.toInt(), value));
	print(u_map);
	ui.insertKeyEdit->clear();
}

void U_map_Visualization::on_eraseBtn_clicked()
{
	QString key = ui.eraseKeyEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key!");
		return;
	}
	u_map.erase(key.toInt());
	print(u_map);
	ui.eraseKeyEdit->clear();
}

void U_map_Visualization::on_itEraseBtn_clicked()
{
	auto iter1 = u_map.begin();
	auto iter2 = u_map.begin();
	QString first = ui.itEraseFirstEdit->text();
	QString second = ui.itEraseSecondEdit->text();
	if (second.isEmpty())
	{
		if (first.isEmpty()) {
			QMessageBox::warning(this, "Warning", "Please fill the first field");
			return;
		}
		std::advance(iter1, first.toInt());
		u_map.erase(iter1);
		print(u_map);
		ui.itEraseFirstEdit->clear();
		ui.itEraseSecondEdit->clear();
		return;
	}
	else {
		if (first.isEmpty() || first.toInt() > second.toInt()) {
			QMessageBox::warning(this, "Warning", "Please fill the first field(first <= second)");
			return;
		}
	}
	std::advance(iter1, first.toInt());
	std::advance(iter2, second.toInt());
	u_map.erase(iter1, iter2);
	print(u_map);
	ui.itEraseFirstEdit->clear();
	ui.itEraseSecondEdit->clear();
}

void U_map_Visualization::on_operatorBtn_clicked()
{
	QString key = ui.operatorKeyEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key!");
		return;
	}
	QString value = u_map[key.toInt()];
	print(u_map);
}

void U_map_Visualization::on_containsBtn_clicked()
{
	QString key = ui.containsKeyEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key!");
		return;
	}
	bool contains = u_map.contains(key.toInt());
	QString bool_str = contains ? "true" : "false";
	ui.containsAnsEdit->setText(bool_str);
	print(u_map);
}

void U_map_Visualization::on_bucket_sizeBtn_clicked()
{
	QString key = ui.bucket_sizeEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key!");
		return;
	}
	size_t bucket_size = u_map.bucket_size(key.toInt());
	ui.bucket_sizeAnsEdit->setText(QString::number(bucket_size));
	print(u_map);
}

void U_map_Visualization::on_bucketBtn_clicked()
{
	QString key = ui.bucketEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Please enter key!");
		return;
	}
	size_t bucket = u_map.bucket(key.toInt());
	ui.bucketAnsEdit->setText(QString::number(bucket));
	print(u_map);
}
