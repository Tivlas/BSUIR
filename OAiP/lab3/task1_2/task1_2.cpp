#include "task1_2.h"
#include "stdafx.h"

task1_2::task1_2(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

bool task1_2::dateValidation(const QString& date)
{
	int leapYear[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (date.size() > 10)
	{
		return false;
	}
	else if (date.size() <= 10)
	{
		foreach(QChar s, date)
		{
			if (s.isDigit() == false && s != '.') {
				return false;
			}
		}

		int day = date.left(2).toInt();
		if (day > 31 || day == 0)
		{
			return false;
		}
		int month = date.mid(3, 2).toInt();
		if (month > 12 || month == 0)
		{
			return false;
		}
		if (leapYear[month - 1] < day) {
			return false;
		}

		int year = date.right(4).toInt();
		if (year == 0 || (!((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) && (day == 29 && month == 2))
		{
			return false;
		}
		return true;
	}
	return true;
}

void task1_2::on_addBtn_clicked()
{
    QString name = ui.nameEdit->text();
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Поле для ввода имени не должно быть пустым");
        return;
    }

    QString s_price = ui.priceEdit->text();
	if (s_price.isEmpty()) {
		QMessageBox::warning(this, "Ошибка", "Поле для ввода цены не должно быть пустым");
		return;
	}
    int price = s_price.toInt();

	QString date = ui.dateEdit->text();
	if (!dateValidation(date)) {
		QMessageBox::warning(this, "Ошибка", "Введите корректную дату");
		return;
	}
	int day = date.left(2).toInt();
	int month = date.mid(3, 2).toInt();
	int year = date.right(4).toInt();

	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	// сегодняшняя дата(системная)
	int t_year = local->tm_year + 1900;
	int t_month = local->tm_mon + 1;
	int t_day = local->tm_mday;
	if (!dateCompare(day, month, year, t_day, t_month, t_year)) {
		QMessageBox::warning(this, "Ошибка", "Введенная дата некорректна");
		return;
	}

	QString s_count = ui.countEdit->text();
	if (s_count.isEmpty()) {
		QMessageBox::warning(this, "Ошибка", "Поле для ввода количества не должно быть пустым");
		return;
	}
	int count = s_count.toInt();

	list.add(Product(name, price, day, month, year, count));
	ui.nameEdit->clear();
	ui.priceEdit->clear();
	ui.dateEdit->clear();
	ui.countEdit->clear();
}

void task1_2::on_printBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString text = list.print();
		ui.plainTextEdit->setPlainText(text);
	}
	else {
		ui.plainTextEdit->setPlainText("");
		QMessageBox::warning(this, "Ошибка", "Список пуст");
	}
}

void task1_2::on_deleteOneByNameBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString name = ui.deleteOneByNameEdit->text();
		if (name.isEmpty()) {
			QMessageBox::warning(this, "Ошибка", "Поле для ввода имени не должно быть пустым");
			return;
		}
		try
		{
			list.remove(name);
		}
		catch (const QString& exception)
		{
			QMessageBox::warning(this, "Ошибка", exception);
			return;
		}
		ui.deleteOneByNameEdit->clear();
		on_printBtn_clicked();
	}
	else {
		ui.deleteOneByNameEdit->clear();
		QMessageBox::warning(this, "Ошибка", "Список пуст");
	}
}

void task1_2::on_deleteAllByNameBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString name = ui.deleteAllByNameEdit->text();
		if (name.isEmpty()) {
			QMessageBox::warning(this, "Ошибка", "Поле для ввода имени не должно быть пустым");
			return;
		}
		try
		{
			list.removeAll(name);
		}
		catch (const QString& exception)
		{
			QMessageBox::warning(this, "Внимание", exception);
			return;
		}
		ui.deleteAllByNameEdit->clear();
		on_printBtn_clicked();
	}
	else {
		ui.deleteAllByNameEdit->clear();
		QMessageBox::warning(this, "Ошибка", "Список пуст");
	}
}

void task1_2::on_quitBtn_clicked() {
	QApplication::quit();
}