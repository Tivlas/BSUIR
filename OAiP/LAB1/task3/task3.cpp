#include "task3.h"
#include "stdafx.h"

task3::task3(QWidget* parent) : QMainWindow(parent)
{
	ui.setupUi(this);
	ui.table->setColumnCount(6);
	ui.table->setHorizontalHeaderLabels(QStringList() << "Date" << "Next day" << "Previous day" << "Week number" <<
		"Is leap?" << "Duration");
}

Date* addToArr(Date* dates, int number)
{
	if (number == 0)
	{
		dates = new Date[number + 1];
	}
	else
	{
		Date* temp = new Date[number + 1];

		for (int i = 0; i < number; i++)
		{
			temp[i] = dates[i];
		}
		delete[] dates;
		dates = temp;
	}
	return dates;
}

bool strValidation(QString str) {

	int leapYear[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

	if (str.size() > 10)
	{
		return false;
	}
	else if (str.size() <= 10)
	{
		foreach(QChar s, str)
		{
			if (s.isDigit() == false && s != '.') {
				return false;
			}
		}

		int day = str.left(2).toInt();
		if (day > 31 || day == 0)
		{
			return false;
		}
		int month = str.mid(3, 2).toInt();
		if (month > 12 || month == 0)
		{
			return false;
		}
		if (leapYear[month-1] < day) {
			return false;
		}

		int year = str.right(4).toInt();
		if (year == 0 || (!((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)) && (day == 29 && month == 2))
		{
			return false;
		}
		return true;
	}
	return true;
}

void task3::on_getBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text) || fromFile.atEnd())
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		for (int i = 0; i <= size; i++) {
			ui.table->removeRow(i);
		}
		ui.table->setRowCount(0);
		delete[] dates;
		size = 0;


		QString text = "";
		bool stop = true;
		for (size = 0; !fromFile.atEnd(); size++)
		{
			ui.table->insertRow(size);
			dates = addToArr(dates, size);

			
			text = fromFile.readLine();
			if (!strValidation(text)) {
				QString wrng = "The file contains invalid data! First error on this line: ";
				wrng += text;
				QMessageBox::critical(this, "Warning", wrng);
				stop = false;
				break;
			}
			int day = text.left(2).toInt();
			int month = text.mid(3, 2).toInt();
			int year = text.right(4).toInt();

			dates[size].setDay(day);
			dates[size].setMonth(month);
			dates[size].setYear(year);
		}
		file.close();

		for (int i = 0; i < size && stop; i++)
		{
			ui.table->setItem(i, 0, dates[i].dateItem());
		}
	}
}

void task3::on_nextBtn_clicked()
{
	for (int i = 0; i < size; i++)
	{
		Date next = dates[i].NextDay();
		ui.table->setItem(i, 1, next.dateItem());
	}
}

void task3::on_previousBtn_clicked()
{

	for (int i = 0; i < size; i++)
	{
		Date prev = dates[i].PreviousDay();
		if (prev.getYear() == 0)
		{
			QTableWidgetItem* item = new QTableWidgetItem(QString("31.12.0001 BC"));
			ui.table->setItem(i, 2, item);
			continue;
		}
		ui.table->setItem(i, 2, prev.dateItem());
	}
}

void task3::on_weekBtn_clicked()
{
	for (int i = 0; i < size; i++)
	{
		short wNum = dates[i].WeekNumber();
		QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(wNum));
		ui.table->setItem(i, 3, item);
	}
}

void task3::on_leapBtn_clicked()
{

	for (int i = 0; i < size; i++)
	{
		bool leap = dates[i].isLeap();
		QString s;
		if (leap)
		{
			s = "Yes, it is.";
		}
		else
		{
			s = "No, it isn't.";
		}
		QTableWidgetItem* item = new QTableWidgetItem(s);
		ui.table->setItem(i, 4, item);
	}
}

void task3::on_durationBtn_clicked()
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	int year = local->tm_year + 1900;
	int month = local->tm_mon + 1;
	int day = local->tm_mday;
	Date now(local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);

	for (int i = 0; i < size; i++)
	{
		int daysBetween = dates[i].Duration(now);
		QTableWidgetItem* item = new QTableWidgetItem(QString("%1 day(days)").arg(daysBetween));
		ui.table->setItem(i, 5, item);
	}
}

void task3::on_birthdayEdit_returnPressed()
{
	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	int year = local->tm_year + 1900;
	int month = local->tm_mon + 1;
	int day = local->tm_mday;
	Date now(local->tm_mday, local->tm_mon + 1, local->tm_year + 1900);

	QString s_date = ui.birthdayEdit->text();
	if (!strValidation(s_date)) {
		QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again.");
		return;
	}
	int birthDay = s_date.left(2).toInt();
	int birthMonth = s_date.mid(3, 2).toInt();
	int birthYear= s_date.right(4).toInt();
	if (birthYear > local->tm_year + 1900) {
		QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again.");
		return;
	}
	Date birthday(birthDay, birthMonth, local->tm_year + 1900);
	if (birthday.dateCompare(now))
	{
		birthday.setYear(local->tm_year + 1901);
	}
	int untilBirth = now.Duration(birthday);

	QString str = QString("%1 day(days)").arg(untilBirth);
	ui.birthLbl->setText(str);
}

void task3::on_quitBtn_clicked()
{
	QApplication::quit();
}

void task3::on_addEdit_returnPressed()
{
	if (size == 0)
	{
		QMessageBox::warning(this, "Warning", "First you need to load the data from the file!");
	}
	else
	{
		QString addDate = ui.addEdit->text();
		if (!strValidation(addDate)) {
			QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again.");
			return;
		}
		int day = addDate.left(2).toInt();
		int month = addDate.mid(3, 2).toInt();
		int year = addDate.right(4).toInt();
		Date newDate(day, month, year);

		dates = addToArr(dates, size);
		dates[size] = newDate;
		ui.table->insertRow(size);
		ui.table->setItem(size, 0, (dates[size]).dateItem());
		size++;
	}
}

void task3::on_saveBtn_clicked()
{
	if (size == 0) {
		QMessageBox::warning(this, "Warning", "First you need to load the data from the file!");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
		{
			QMessageBox::warning(this, "Warning", "The file was not opened!");
		}
		else
		{

			QString text = dates[size - 1].dateToString();
			toFile << "\n" << text;
			file.flush();
			file.close();
		}
	}
	
}

void task3::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text) || fromFile.atEnd())
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		QString text = fromFile.readAll();
		ui.plainTextEdit->setPlainText(text);
		file.close();
	}
}

void task3::on_writeBtn_clicked()
{

	fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream toFile(&file);

	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(this, "Warning", "The file was not opened!");
	}
	else
	{
		QString text = ui.plainTextEdit->toPlainText();
		toFile << text;
		file.flush();
		file.close();
	}
}
