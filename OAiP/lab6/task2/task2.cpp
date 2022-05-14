#include "task2.h"
#include "stdafx.h"

task2::task2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void task2::print(const HashTable& table)
{
	ui.textEdit->clear();
	QString str = "";
	for (int i = 0; i < table.buckets_count(); i++)
	{
		List<int> temp = table.get_bucket(i);
		if (!temp.empty())
		{
			str += "Bucket #" + QString::number(i) + ":  ";
			for (auto it = temp.begin(); it != temp.end(); ++it)
			{
				str += QString::number(*it) + " -> ";
			}
			str.remove(str.size() - 4, 4);
			ui.textEdit->append(str);
			str = "";
		}
		else {
			ui.textEdit->append("Bucket #" + QString::number(i) + ":  EMPTY");
		}
	}
}

void task2::on_insertBtn_clicked()
{
	static size_t click_count = 1;
	ui.insertEdit->setText(QString::number(click_count));
	++click_count;
	int key = rand() % 1000;
	hash_table.insert(key);
	print(hash_table);
}

void task2::on_removeBtn_clicked()
{
	QString key = ui.removeEdit->text();
	if (key.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Enter key");
		return;
	}
	hash_table.remove(key.toInt());
	print(hash_table);
}

void task2::on_findBtn_clicked()
{
	int min = INT_MAX;
	size_t min_bucket_index = 0;
	bool min_found = false;
	for (int i = 0; i < hash_table.buckets_count(); i++)
	{
		List<int> temp = hash_table.get_bucket(i);
		if (!temp.empty())
		{
			for (auto it = temp.begin(); it != temp.end(); ++it)
			{
				if (*it < min) {
					min = *it;
					min_bucket_index = i;
				}
			}
			min_found = true;
		}
	}
	if (min_found)
	{
		ui.findEdit->setText("№: " + QString::number(min_bucket_index) + ", value: " + QString::number(min));
	}
	else {
		QMessageBox::warning(this, "Warning", "No elements in table");
	}
}
