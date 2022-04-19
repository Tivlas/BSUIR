#include "task1_1.h"
#include "stdafx.h"

task1_1::task1_1(QWidget* parent)

	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.saveLastBtn->setEnabled(false);
	ui.saveAllBtn->setEnabled(false);
	this->setFixedHeight(585);
	this->setFixedWidth(1074);
}

// validation
bool task1_1::dateValidation(const QString& date)
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

bool task1_1::depNumberValidation(const QString& depNumber)
{
	if (depNumber.isEmpty())
	{
		return false;
	}
	else
	{
		for (int i = 0; i < depNumber.length(); i++)
		{
			if (!depNumber[i].isDigit())
			{
				return false;
			}
		}
		return true;
	}
}

bool task1_1::dateCompare(int day1, int month1, int year1, int day2, int month2, int year2)
{
	QString first_year;
	first_year.setNum(year1);
	QString first_month;
	first_month.setNum(month1);
	QString first_day;
	first_day.setNum(day1);
	if (month1 / 10 == 0) {
		first_month.prepend('0');
	}
	if (day1 / 10 == 0) {
		first_day.prepend('0');
	}
	if (year1 < 10)
	{
		first_year.prepend("000");
	}
	else if (year1 < 100)
	{
		first_year.prepend("00");
	}
	else if (year1 < 1000)
	{
		first_year.prepend('0');
	}
	QString full_first_date = first_year + first_month + first_day;


	QString second_year;
	second_year.setNum(year2);
	QString second_month;
	second_month.setNum(month2);
	QString second_day;
	second_day.setNum(day2);

	if (month2 / 10 == 0) {
		second_month.prepend('0');
	}
	if (day2 / 10 == 0) {
		second_day.prepend('0');
	}
	if (year2 < 10)
	{
		second_year.prepend("000");
	}
	else if (year2 < 100)
	{
		second_year.prepend("00");
	}
	else if (year2 < 1000)
	{
		second_year.prepend('0');
	}
	QString full_second_date = second_year + second_month + second_day;

	bool cmp = (full_second_date >= full_first_date) ? 1 : 0;
	return cmp;
}

void task1_1::on_quitBtn_clicked() {
	QApplication::quit();
}

// add employees to list
void task1_1::on_readToListBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/BSUIR/OAiP/lab3/task1_1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);
	QString text = "";
	if (!file.open(QFile::ReadOnly | QFile::Text) || fromFile.atEnd())
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		if (!list.getHead()) {
			list.clear();
			ui.saveLastBtn->setEnabled(false);
			ui.saveAllBtn->setEnabled(false);
		}
		int linesCounter = 1;
		while (!fromFile.atEnd()) {
			Employee* cur = new Employee();

			text = fromFile.readLine(); // name
			++linesCounter;
			cur->setName(text);

			text = fromFile.readLine(); // surname
			++linesCounter;
			cur->setSurname(text);

			text = fromFile.readLine(); // patronymic
			++linesCounter;
			cur->setPatronymic(text);

			text = fromFile.readLine(); // employment date
			++linesCounter;
			if (!dateValidation(text)) {
				QString error = "Error in line " + QString::number(linesCounter);
				QMessageBox::critical(this, "Error", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}
			int s_day = text.left(2).toInt();
			int s_month = text.mid(3, 2).toInt();
			int s_year = text.right(4).toInt();
			struct tm* local;
			time_t t;
			t = time(NULL);
			local = localtime(&t);
			// today's date
			int year = local->tm_year + 1900;
			int month = local->tm_mon + 1;
			int day = local->tm_mday;
			if (!dateCompare(s_day, s_month, s_year, day, month, year)) {
				QString error = "Error in line " + QString::number(linesCounter);
				QMessageBox::critical(this, "Error", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}
			cur->setS_day(s_day);
			cur->setS_month(s_month);
			cur->setS_year(s_year);

			text = fromFile.readLine(); // department number
			++linesCounter;
			if (!depNumberValidation(text)) {
				QString error = "Error in line " + QString::number(linesCounter);
				QMessageBox::critical(this, "Error", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}
			cur->setDepartmentNumber(text.toInt());

			text = fromFile.readLine(); // position
			++linesCounter;
			cur->setPosition(text);

			list.add(cur);
		}
		file.close();
		ui.saveAllBtn->setEnabled(true);
		on_printBtn_clicked();
	}
}


void task1_1::on_addBtn_clicked()
{

	QString name = ui.nameEdit->text();
	if (name.isEmpty()) {
		QMessageBox::warning(this, "Error", "Name is empty");
		return;
	}

	QString surname = ui.surnameEdit->text();
	if (surname.isEmpty()) {
		QMessageBox::warning(this, "Error", "Surname is empty");
		return;
	}

	QString patronymic = ui.patronymicEdit->text();
	if (patronymic.isEmpty()) {
		QMessageBox::warning(this, "Error", "Patronymic is empty");
		return;
	}

	QString departmentNumber = ui.departmentNumberEdit->text();
	if (departmentNumber.isEmpty()) {
		QMessageBox::warning(this, "Error", "Department number is empty");
		return;
	}

	QString position = ui.positionEdit->text();
	if (position.isEmpty()) {
		QMessageBox::warning(this, "Error", "Position is empty");
		return;
	}

	QString startDate = ui.startDateEdit->text();
	if (!dateValidation(startDate)) {
		QMessageBox::warning(this, "Error", "Start date is incorrect");
		return;
	}
	int s_day = startDate.left(2).toInt();
	int s_month = startDate.mid(3, 2).toInt();
	int s_year = startDate.right(4).toInt();

	struct tm* local;
	time_t t;
	t = time(NULL);
	local = localtime(&t);
	// today's date
	int year = local->tm_year + 1900;
	int month = local->tm_mon + 1;
	int day = local->tm_mday;
	if (!dateCompare(s_day, s_month, s_year, day, month, year)) {
		QMessageBox::warning(this, "Error", "Start date is incorrect");
		return;
	}

	list.add(new Employee(departmentNumber.toInt(), position, name, surname, patronymic, s_day, s_month, s_year));

	ui.nameEdit->clear();
	ui.surnameEdit->clear();
	ui.patronymicEdit->clear();
	ui.departmentNumberEdit->clear();
	ui.startDateEdit->clear();
	ui.positionEdit->clear();
	ui.saveLastBtn->setEnabled(true);
	ui.saveAllBtn->setEnabled(true);
	on_printBtn_clicked();
}


// print
void task1_1::on_printBtn_clicked()
{
	if (!list.isEmpty())
	{
		QString text = list.print();
		ui.plainTextEdit->setPlainText(text);
	}
	else {
		QMessageBox::warning(this, "Error", "List is empty");
		ui.plainTextEdit->setPlainText("");
	}
}

void task1_1::on_printEmployeesWithSpecificSurnameBtn_clicked()
{
	if (!list.isEmpty()) {
		QString surname = ui.printSurnameEdit->text();
		QString text;
		if (surname.isEmpty()) {
			QMessageBox::warning(this, "Error", "Surname is empty");
			return;
		}
		try
		{
			text = list.printEmployeesWithSpecificSurname(surname);
		}
		catch (const QString& warning)
		{
			QMessageBox::warning(this, "Warning", warning);
			return;
		}
		ui.plainTextEdit->setPlainText(text);
	}
	else {
		QMessageBox::warning(this, "Error", "List is empty");
		ui.plainTextEdit->setPlainText("");
	}
}

void task1_1::on_printEmployeesWithSpecificDepNumberBtn_clicked()
{
	if (!list.isEmpty()) {
		QString depNumber = ui.printDepNumberEdit->text();
		QString text;
		if (depNumber.isEmpty()) {
			QMessageBox::warning(this, "Error", "Department number is empty");
			return;
		}
		try
		{
			text = list.printEmployeesWithSpecificDepNumber(depNumber.toInt());
		}
		catch (const QString& warning)
		{
			QMessageBox::warning(this, "Warning", warning);
			return;
		}
		ui.plainTextEdit->setPlainText(text);
	}
	else {
		QMessageBox::warning(this, "Error", "List is empty");
		ui.plainTextEdit->setPlainText("");
	}
}

void task1_1::on_printExperienceBtn_clicked()
{
	if (!list.isEmpty()) {
		if (ui.printExperienceEdit->text().isEmpty()) {
			QMessageBox::warning(this, "Error", "Experience is empty");
			return;
		}
		int experience = ui.printExperienceEdit->text().toInt();

		// today's date
		struct tm* local;
		time_t t;
		t = time(NULL);
		local = localtime(&t);
		int year = local->tm_year + 1900;
		int month = local->tm_mon + 1;
		int day = local->tm_mday;

		QString result = "";
		Employee* cur = list.getHead();
		while (cur != nullptr) {
			int dur = duration(cur->getS_day(), cur->getS_month(), cur->getS_year(), day, month, year);
			if (dur >= experience) {
				result += cur->getAllInformation();
			}
			cur = cur->getNext();
		}
		if (result.isEmpty()) {
			QMessageBox::warning(this, "Warning", "There are no employees with such experience");
			return;
		}
		ui.plainTextEdit->setPlainText(result);
	}
	else {
		QMessageBox::warning(this, "Warning", "List is empty");
		ui.plainTextEdit->setPlainText("");
	}
}

// fire
void task1_1::on_fireBtn_clicked()
{
	if (!list.isEmpty())
	{
		QString name = ui.fireNameEdit->text();
		if (name.isEmpty()) {
			QMessageBox::warning(this, "Error", "Name is empty");
			return;
		}

		QString surname = ui.fireSurnameEdit->text();
		if (surname.isEmpty()) {
			QMessageBox::warning(this, "Error", "Surname is empty");
			return;
		}

		QString patronymic = ui.firePatronymicEdit->text();
		if (patronymic.isEmpty()) {
			QMessageBox::warning(this, "Error", "Patronymic is empty");
			return;
		}

		QString departmentNumber = ui.fireDepartmentNumberEdit->text();
		if (departmentNumber.isEmpty()) {
			QMessageBox::warning(this, "Error", "Department number is empty");
			return;
		}

		try
		{
			list.remove(name, surname, patronymic, departmentNumber.toInt());
		}
		catch (const QString& warning)
		{
			QMessageBox::warning(this, "Warning", warning);
			return;
		}
		ui.fireNameEdit->clear();
		ui.fireSurnameEdit->clear();
		ui.firePatronymicEdit->clear();
		ui.fireDepartmentNumberEdit->clear();
		on_printBtn_clicked();
	}
	else {
		QMessageBox::warning(this, "Warning", "List is empty");
	}

}

// save
void task1_1::on_saveLastBtn_clicked()
{
	if (list.isEmpty()) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/BSUIR/OAiP/lab3/task1_1", "Text File (*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
		{
			QMessageBox::warning(this, "Warning", "The file was not opened!");
		}
		else
		{

			QString text = list.getTail()->getAllInformation();
			toFile << text;
			file.flush();
			file.close();
		}
	}
}

void task1_1::on_saveAllBtn_clicked()
{
	if (list.isEmpty()) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/BSUIR/OAiP/lab3/task1_1", "Text File (*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text))
		{
			QMessageBox::warning(this, "Warning", "The file was not opened!");
		}
		else
		{
			QString text = list.print();
			toFile << text;
			file.flush();
			file.close();
		}
	}
}

// duration
int task1_1::daysPassed(int day, int month, int year)
{
	int leapYear[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int notLeapYear[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	if (month == 1)
	{
		return day;
	}
	else
	{
		int daysPassed = -1; // number of days that have passed until the current month
		if ((year % 4 == 0 && year % 100 != 0) || year % 400 == 0)
		{
			for (int i = 0; i < month - 1; i++)
			{
				daysPassed += leapYear[i];
			}
		}
		else
		{
			for (int i = 0; i < month - 1; i++)
			{
				daysPassed += notLeapYear[i];
			}
		}

		return daysPassed + day;
	}
}

int task1_1::duration(int day1, int month1, int year1, int day2, int month2, int year2)
{
	// second date is current system date
	if (dateCompare(day1, month1, year1, day2, month2, year2))
	{
		int daysBetweenYears = 0; // 2019-2023(2020+2021+2022)
		int untilNextYear = 0; // from current date to next year
		int days_passed = 0; // from the beginning of the year until given date
		days_passed = daysPassed(day2, month2, year2);

		if (year2 - year1 >= 1)
		{
			if ((year1 % 4 == 0 && year1 % 100 != 0) || year1 % 400 == 0) {
				untilNextYear = 366 - daysPassed(day1, month1, year1);
			}
			else {
				untilNextYear = 365 - daysPassed(day1, month1, year1);
			}

			for (int i = year1 + 1; i < year2; i++)
			{
				if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0)
				{
					daysBetweenYears += 366;
				}
				else
				{
					daysBetweenYears += 365;
				}
			}
			int res = untilNextYear + days_passed + daysBetweenYears;
			return res;
		}
		else {
			untilNextYear = daysPassed(day1, month1, year1);
			int res = abs(untilNextYear - days_passed);
			return res;
		}

	}
	else
	{
		return duration(day2, month2, year2, day1, month1, year1);
	}
}

// sortings
void task1_1::sortByDepNumber(List& list, int left, int right)
{
	int l = left;
	int r = right;
	int mid = list.getElement((left + right) / 2)->getDepartmentNumber();
	while (l < r)
	{
		while (list.getElement(l)->getDepartmentNumber() < mid)
		{
			l++;
		}
		while (list.getElement(r)->getDepartmentNumber() > mid)
		{
			r--;
		}
		if (l <= r)
		{
			list.swap(l, r);
			++l;
			--r;
		}
	}
	if (left < l)
	{
		sortByDepNumber(list, left, r);
	}
	if (r < right)
	{
		sortByDepNumber(list, l, right);
	}
}

void task1_1::sortBySurname(List& list, int left, int right)
{
	int l = left;
	int r = right;
	QString mid = list.getElement((left + right) / 2)->getSurname().toUpper();
	while (l < r)
	{
		while (list.getElement(l)->getSurname().toUpper() < mid)
		{
			l++;
		}
		while (list.getElement(r)->getSurname().toUpper() > mid)
		{
			r--;
		}
		if (l <= r)
		{
			list.swap(l, r);
			++l;
			--r;
		}
	}
	if (left < l)
	{
		sortBySurname(list, left, r);
	}
	if (r < right)
	{
		sortBySurname(list, l, right);
	}
}

void task1_1::sortByEmploymentDate(List& list, int left, int right)
{
	int l = left;
	int r = right;
	QString mid = list.getElement((left + right) / 2)->startDateToStringWithoutDot();
	while (l < r)
	{
		while (list.getElement(l)->startDateToStringWithoutDot() < mid)
		{
			l++;
		}
		while (list.getElement(r)->startDateToStringWithoutDot() > mid)
		{
			r--;
		}
		if (l <= r)
		{
			list.swap(l, r);
			++l;
			--r;
		}
	}
	if (left < l)
	{
		sortByEmploymentDate(list, left, r);
	}
	if (r < right)
	{
		sortByEmploymentDate(list, l, right);
	}
}

void task1_1::on_sortByDepNumberBtn_clicked()
{
	if (!list.isEmpty())
	{
		sortByDepNumber(list, 0, list.getSize() - 1);
		QString result = list.print();
		ui.plainTextEdit->setPlainText(result);
	}
	else {
		QMessageBox::warning(this, "Warning", "List is empty");
	}
}

void task1_1::on_sortBySurnameBtn_clicked()
{
	if (!list.isEmpty())
	{
		sortBySurname(list, 0, list.getSize() - 1);
		QString result = list.print();
		ui.plainTextEdit->setPlainText(result);
	}
	else {
		QMessageBox::warning(this, "Warning", "List is empty");
	}
}

void task1_1::on_sortByEmploymentDateBtn_clicked()
{
	if (!list.isEmpty())
	{
		sortByEmploymentDate(list, 0, list.getSize() - 1);
		QString result = list.print();
		ui.plainTextEdit->setPlainText(result);
	}
	else {
		QMessageBox::warning(this, "Warning", "List is empty");
	}
}

// read from file to plainTextEdit
void task1_1::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/BSUIR/OAiP/lab3/task1_1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text))
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

void task1_1::on_overwriteBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/BSUIR/OAiP/lab3/task1_1", "Text File (*.txt)");
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
