#include "task1_2.h"
#include "stdafx.h"

task1_2::task1_2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	ui.saveLastBtn->setEnabled(false);
	ui.saveAllBtn->setEnabled(false);
}

bool task1_2::amountValidation(const QString& amount)
{
	if (amount.isEmpty())
	{
		return false;
	}
	else
	{
		for (int i = 0; i < amount.length(); i++)
		{
			if (!amount[i].isDigit())
			{
				return false;
			}
		}
		return true;
	}
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

// добавление
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
	ui.saveLastBtn->setEnabled(true);
	ui.saveAllBtn->setEnabled(true);
	on_printBtn_clicked();
}

void task1_2::on_readToListBtn_clicked()
{
	if (list.getActualSize() != 0) {
		list.clear();
	}
	fileName = QFileDialog::getOpenFileName(this, "Откройте файл", "D:/BSUIR/OAiP/lab3/task1_2", "(*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);
	QString name;
	QString s_price;
	QString date;
	QString amount;
	if (!file.open(QFile::ReadOnly | QFile::Text) || fromFile.atEnd())
	{
		QMessageBox::warning(this, "Внимание", "Файл не был открыт или он пустой!");
	}
	else
	{
		if (list.getActualSize() != 0) {
			list.clear();
			ui.saveLastBtn->setEnabled(false);
			ui.saveAllBtn->setEnabled(false);
		}
		int linesCounter = 1;
		while (!fromFile.atEnd()) {

			name = fromFile.readLine(); // name
			++linesCounter;
			if (name.isEmpty()) {
				QString error = "Ошибка в строке " + QString::number(linesCounter);
				QMessageBox::critical(this, "Ошибка", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}

			s_price = fromFile.readLine(); // s_price
			++linesCounter;
			if (s_price.isEmpty()) {
				QString error = "Ошибка в строке " + QString::number(linesCounter);
				QMessageBox::critical(this, "Ошибка", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}

			date = fromFile.readLine(); //  date
			++linesCounter;
			if (!dateValidation(date)) {
				QString error = "Ошибка в строке " + QString::number(linesCounter);
				QMessageBox::critical(this, "Ошибка", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}
			int day = date.left(2).toInt();
			int month = date.mid(3, 2).toInt();
			int year = date.right(4).toInt();
			struct tm* local;
			time_t t;
			t = time(NULL);
			local = localtime(&t);
			// today's date
			int t_year = local->tm_year + 1900;
			int t_month = local->tm_mon + 1;
			int t_day = local->tm_mday;
			if (!dateCompare(day, month, year, t_day, t_month, t_year)) {
				QString error = "Ошибка в строке " + QString::number(linesCounter);
				QMessageBox::critical(this, "Ошибка", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}

			amount = fromFile.readLine(); // amount
			++linesCounter;
			if (!amountValidation(amount)) {
				QString error = "Error in line " + QString::number(linesCounter);
				QMessageBox::critical(this, "Error", error);
				list.clear();
				ui.saveLastBtn->setEnabled(false);
				ui.saveAllBtn->setEnabled(false);
				return;
			}

			list.add(Product(name, s_price.toInt(), day, month, year, amount.toInt()));
		}
		file.close();
		ui.saveAllBtn->setEnabled(true);
		on_printBtn_clicked();
	}
}

// вывод
void task1_2::on_printBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString text = list.print();
		ui.plainTextEdit->setPlainText(text);
	}
	else {
		ui.plainTextEdit->setPlainText("");
		QMessageBox::warning(this, "Внимание!", "Список пуст");
	}
}

void task1_2::on_printByPriceBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString s_price = ui.printByPriceEdit->text();
		QString text;
		if (s_price.isEmpty()) {
			QMessageBox::warning(this, "Ошибка", "Поле для ввода цены не должно быть пустым");
			return;
		}
		try
		{
			text = list.printProductsWithLowerPrice(s_price.toInt());
		}
		catch (const QString& exception)
		{
			QMessageBox::warning(this, "Ошибка", exception);
			return;
		}
		ui.plainTextEdit->setPlainText(text);
		ui.printByPriceEdit->clear();
	}
	else {
		ui.printByPriceEdit->clear();
		QMessageBox::warning(this, "Внимание!", "Список пуст");
	}
}

void task1_2::on_printByNameBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		QString name = ui.printByNameEdit->text();
		QString text;
		if (name.isEmpty()) {
			QMessageBox::warning(this, "Ошибка", "Поле для ввода имени не должно быть пустым");
			return;
		}
		try
		{
			text = list.printByProductName(name);
		}
		catch (const QString& exception)
		{
			QMessageBox::warning(this, "Ошибка", exception);
			return;
		}
		ui.plainTextEdit->setPlainText(text);
		ui.printByNameEdit->clear();
	}
	else {
		ui.printByNameEdit->clear();
		QMessageBox::warning(this, "Внимание!", "Список пуст");
	}
}

void task1_2::on_printByTimeBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		if (ui.printByTimeEdit->text().isEmpty()) {
			QMessageBox::warning(this, "Ошибка", "Поле для ввода колчества дней не должно быть пустым");
			return;
		}
		int days = ui.printByTimeEdit->text().toInt();
		QString text;
		try
		{
			text = list.printByStorageTime(days);
		}
		catch (const QString& exception)
		{
			QMessageBox::warning(this, "Ошибка", exception);
			return;
		}
		ui.plainTextEdit->setPlainText(text);
		ui.printByTimeEdit->clear();
	}
	else {
		ui.printByTimeEdit->clear();
		QMessageBox::warning(this, "Внимание!", "Список пуст");
	}
}


// сохранение
void task1_2::on_saveLastBtn_clicked()
{
	if (list.getActualSize() == 0) {
		QMessageBox::warning(this, "Ошибка", "Список пуст");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Выберите файл для сохранения", "D:/BSUIR/OAiP/lab3/task1_2", "(*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
		{
			QMessageBox::warning(this, "Ошибка", "Файл не был открыт");
		}
		else
		{

			QString text = list.getInfoAboutLast();
			toFile << text;
			file.flush();
			file.close();
		}
	}
}

void task1_2::on_saveAllBtn_clicked()
{
	if (list.getActualSize() == 0) {
		QMessageBox::warning(this, "Ошибка", "Список пуст");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Выберите файл для сохранения", "D:/BSUIR/OAiP/lab3/task1_2", "(*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text))
		{
			QMessageBox::warning(this, "Ошибка", "Файл не был открыт");
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


// удаление
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
		QMessageBox::warning(this, "Внимание!", "Список пуст");
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
		QMessageBox::warning(this, "Внимание!", "Список пуст");
	}
}

// выход
void task1_2::on_quitBtn_clicked() {
	QApplication::quit();
}

// работа с plainTextEdit
void task1_2::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "D:/BSUIR/OAiP/lab3/task1_2", "(*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "Внимание", "Файл не был открыт или он пустой!");
	}
	else
	{
		QString text = fromFile.readAll();
		ui.plainTextEdit->setPlainText(text);
		file.close();
	}
}

void task1_2::on_overwriteBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Выберите файл", "D:/BSUIR/OAiP/lab3/task1_2", "(*.txt)");
	QFile file(fileName);
	QTextStream toFile(&file);

	if (!file.open(QFile::WriteOnly | QFile::Text))
	{
		QMessageBox::warning(this, "Внимание", "Файл не был открыт!");
	}
	else
	{
		QString text = ui.plainTextEdit->toPlainText();
		toFile << text;
		file.flush();
		file.close();
	}
}

// сортировка
void task1_2::on_sortByPriceBtn_clicked()
{
	if (list.getActualSize() != 0)
	{
		list.sortByPrice();
		on_printBtn_clicked();
	}
	else {
		QMessageBox::warning(this, "Ошибка!", "Список пуст");
	}
}
