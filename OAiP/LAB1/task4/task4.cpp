#include "task4.h"
#include "stdafx.h"

task4::task4(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

bool winSessioFromFileValid(QString s, Student& st) {

	for (int i = 0; i < s.size(); i++) {
		if (s[i].isDigit() == false && (s[i] != ',' && s[i] != ' ')) {
			return false;
		}
	}
	QStringList list = s.split(QLatin1Char(','), Qt::SkipEmptyParts);
	//arr = new int[list.size()];

	st.winMarks = new int[list.size()];
	*(st.setWinSize()) = list.size();
	for (int i = 0; i < list.size(); i++) {
		st.winMarks[i] = list[i].toInt();
	}
	for (int i = 0; i < list.size(); i++) {
		if (st.winMarks[i] > 10) {
			delete[] st.winMarks;
			*(st.setWinSize()) = 0;
			return false;
		}
	}
	int sum = 0;
	for (int i = 0; i < list.size(); i++) {
		sum += st.winMarks[i];
	}
	*(st.setWinAvg()) = static_cast<double>(sum) / list.size();

	return true;
}

bool sumSessioFromFileValid(QString s, Student& st) {

	for (int i = 0; i < s.size(); i++) {
		if (s[i].isDigit() == false && (s[i] != ',' && s[i] != ' ')) {
			return false;
		}
	}
	QStringList list = s.split(QLatin1Char(','), Qt::SkipEmptyParts);
	//arr = new int[list.size()];

	st.sumMarks = new int[list.size()];
	*(st.setSumSize()) = list.size();
	for (int i = 0; i < list.size(); i++) {
		st.sumMarks[i] = list[i].toInt();
	}
	for (int i = 0; i < list.size(); i++) {
		if (st.sumMarks[i] > 10) {
			delete[] st.sumMarks;
			*(st.setSumSize());
			return false;
		}
	}
	int sum = 0;
	for (int i = 0; i < list.size(); i++) {
		sum += st.sumMarks[i];
	}
	*(st.setSumAvg()) = static_cast<double>(sum) / list.size();

	return true;
}

// валидация номера группы
bool grNumValidation(QString str) {
	if (str.size() != 6)
	{
		return false;
	}
	else if (str.size() == 6)
	{
		foreach(QChar s, str)
		{
			if (s.isDigit() == false) {
				return false;
			}
		}
		return true;
	}
	return true;
}

// добавление в массив
Student* addToArr(Student* students, int number)
{
	if (number == 0)
	{
		students = new Student[number + 1];
	}
	else
	{
		Student* temp = new Student[number + 1];

		for (int i = 0; i < number; i++)
		{
			temp[i].initialize(students[i]);
		}
		delete[] students;
		students = temp;
	}
	return students;
}

void task4::on_getBtn_clicked()
{
	ui.saveBtn->setEnabled(false);
	ui.nameEdit->setEnabled(false);
	ui.groupNumberEdit->setEnabled(false);
	ui.winterSessionEdit->setEnabled(false);
	ui.summerSessionEdit->setEnabled(false);
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream fromFile(&file);
	QString text = "";
	if (!file.open(QFile::ReadOnly | QFile::Text) || fromFile.atEnd())
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		bool fail = false;
		if (studentsSize != 0) {
			delete[] students;
		}
		for (studentsSize = 0; !fromFile.atEnd(); studentsSize++)
		{

			students = addToArr(students, studentsSize);
			text = fromFile.readLine(); // номер группы
			if (!grNumValidation(text)) {
				QString wrng = "The file contains invalid data! First error on this line: ";
				wrng += text;
				QMessageBox::critical(this, "Warning", wrng);
				fail = true;
				break;
			}
			*(students[studentsSize].setGroupNum()) = text.toInt();

			text = fromFile.readLine(); // имя
			*(students[studentsSize].setName()) = text;

			text = fromFile.readLine(); // зимняя сессия
			if (!winSessioFromFileValid(text, students[studentsSize])) {
				QString wrng = "The file contains invalid data! First error on this line: ";
				wrng += text;
				QMessageBox::critical(this, "Warning", wrng);
				fail = true;
				break;
			}

			text = fromFile.readLine(); // летняя сессия
			if (!sumSessioFromFileValid(text, students[studentsSize])) {
				QString wrng = "The file contains invalid data! First error on this line: ";
				wrng += text;
				QMessageBox::critical(this, "Warning", wrng);
				fail = true;
				break;
			}
		}
		file.close();

		if (fail) {
			delete[] students;
			studentsSize = 0;
		}
	}
}


// добавление в массив оценок
int* addToMarksArr(int* arr, int number) {
	if (number == 0)
	{
		arr = new int[number + 1];
	}
	else
	{
		int* temp = new int[number + 1];

		for (int i = 0; i < number; i++)
		{
			temp[i] = arr[i];
		}
		delete[] arr;
		arr = temp;
	}
	return arr;
}

   
// валидация оценок  за сессию
bool sessionValidation(QString str) {


	if (str.size() > 2 || str.size() == 0) {
		return false;
	}
	for (int i = 0; i < str.size(); i++) {
		if (str[i].isDigit() == false) {
			return false;
		}
	}
	if (str.toInt() > 10) {
		return false;
	}
	return true;
}

// сортировка по номеру группы
void task4::groupSort(Student*& arr, int left, int right)
{
	int l = left, r = right;
	int index = (r + l) / 2;
	Student temp, mid;
	mid.initialize(arr[index]);

	while (l < r) {
		while (arr[l].getGrNum() < mid.getGrNum()) {
			l++;
		}
		while (arr[r].getGrNum() > mid.getGrNum()) {
			r--;
		}
		if (l <= r) {
			temp.initialize(arr[l]); arr[l].initialize(arr[r]); arr[r].initialize(temp);
			l++; r--;
		}
		if (left < l) {
			groupSort(arr, left, r);
		}
		if (r < right) {
			groupSort(arr, l, right);
		}
	}
}

// сортировка по имени
void task4::fullNameSort(Student*& arr, int left, int right)
{
	int l = left, r = right;
	int index = (r + l) / 2;
	Student temp, mid;
	mid.initialize(arr[index]);

	while (l < r) {
		while (arr[l].getName().toUpper() < mid.getName().toUpper()) {
			l++;
		}
		while (arr[r].getName().toUpper() > mid.getName().toUpper()) {
			r--;
		}
		if (l <= r) {
			temp.initialize(arr[l]); arr[l].initialize(arr[r]); arr[r].initialize(temp);
			l++; r--;
		}
		if (left < l) {
			fullNameSort(arr, left, r);
		}
		if (r < right) {
			fullNameSort(arr, l, right);
		}
	}
}

// поиск студента по имени
int task4::findStudent(Student* arr, int size, QString key)
{
	for (int i = 0; i < size; i++) {
		if (arr[i].getName() == key) {
			return i;
		}
	} return -1;
}

// поиск должников(создание массива)
Student* task4::findDebtors(Student*& debt)
{
	for (int i = 0; i < studentsSize; i++) {
		students[i].markSort(students[i].winMarks, 0, students[i].getWinSize() - 1);
		if (students[i].findBadMark(students[i].winMarks, students[i].getWinSize())) {
			debt = addToArr(debt, debtorsSize);
			debtorsSize++;
			debt[debtorsSize - 1].initialize(students[i]);
		}
	}
	return debt;
}

// поиск студентов на отчисление(создание массива)
Student* task4::findDrop(Student*& drop)
{
	for (int i = 0; i < studentsSize; i++) {
		students[i].markSort(students[i].winMarks, 0, students[i].getWinSize() - 1);
		students[i].markSort(students[i].sumMarks, 0, students[i].getSumSize() - 1);
		if (students[i].findBadMark(students[i].winMarks, students[i].getWinSize()) &&
			students[i].findTwoBadMarks(students[i].sumMarks, students[i].getSumSize())) {
			drop = addToArr(drop, dropoutSize);
			dropoutSize++;
			drop[dropoutSize - 1].initialize(students[i]);
		}
	}
	return drop;
}

// ввод имени
void task4::on_nameEdit_returnPressed()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {

		QString name = ui.nameEdit->text();
		*(students[studentsSize - 1].setName()) = name;
	}

}

// ввод номера группы
void task4::on_groupNumberEdit_returnPressed()
{

	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {

		QString grNumber = ui.groupNumberEdit->text();
		if (!grNumValidation(grNumber)) {
			QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again!");
		}
		else {

			int x = grNumber.toInt();
			*(students[studentsSize - 1].setGroupNum()) = x;
		}
	}

}

// ввод оценок за зимнюю сессию
void task4::on_winterSessionEdit_returnPressed()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {


		QString text = ui.winterSessionEdit->text();
		if (!sessionValidation(text)) {
			QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again!");
		}
		else {

			int size = students[studentsSize - 1].getWinSize();
			students[studentsSize - 1].winMarks = addToMarksArr(students[studentsSize - 1].winMarks, size);
			students[studentsSize - 1].winMarks[size] = text.toInt();
			(*(students[studentsSize - 1].setWinSize()))++;
			size++;
			*(students[studentsSize - 1].setWinterSum()) += text.toInt();
			*(students[studentsSize - 1].setWinAvg()) = static_cast<double>(students[studentsSize - 1].getWinterSum()) / size;
		}
	}

}

// ввод оценок за летнюю сессию
void task4::on_summerSessionEdit_returnPressed()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {

		QString text = ui.summerSessionEdit->text();
		if (!sessionValidation(text)) {
			QMessageBox::warning(this, "Warning", "You have entered incorrect data! Try again!");
		}
		else {

			int size = students[studentsSize - 1].getSumSize();
			students[studentsSize - 1].sumMarks = addToMarksArr(students[studentsSize - 1].sumMarks, size);
			students[studentsSize - 1].sumMarks[size] = text.toInt();
			(*(students[studentsSize - 1].setSumSize()))++;
			size++;
			*(students[studentsSize - 1].setSummerSum()) += text.toInt();
			*(students[studentsSize - 1].setSumAvg()) = static_cast<double>(students[studentsSize - 1].getSummerSum()) / size;
		}
	}

}

// добавить студента
void task4::on_addstudentBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First you need to load the data from the file!");
	}
	else {
		ui.saveBtn->setEnabled(true);
		ui.nameEdit->setEnabled(true);
		ui.groupNumberEdit->setEnabled(true);
		ui.winterSessionEdit->setEnabled(true);
		ui.summerSessionEdit->setEnabled(true);


		students = addToArr(students, studentsSize);
		studentsSize++;
	}
}

// сохранить добавленного студента в файл
void task4::on_saveBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {
		ui.saveBtn->setEnabled(false);
		fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
		{
			QMessageBox::warning(this, "Warning", "The file was not opened!");
		}
		else
		{
			ui.winterSessionEdit->setEnabled(false);
			ui.summerSessionEdit->setEnabled(false);
			ui.nameEdit->setEnabled(false);
			ui.groupNumberEdit->setEnabled(false);

			QString text = students[studentsSize - 1].infoToString();
			toFile << "\n" << text;
			file.flush();
			file.close();
		}
	}
}

// сохранить массив из всех студентов в файл
void task4::on_saveAllBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First create an array!");
	}
	else {
		ui.winterSessionEdit->setEnabled(false);
		ui.summerSessionEdit->setEnabled(false);
		ui.nameEdit->setEnabled(false);
		ui.groupNumberEdit->setEnabled(false);
		arrToFile(students, studentsSize);
	}
}

// прочитать из любого файла
void task4::on_readBtn_clicked() {
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
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

// перезаписать любой файл
void task4::on_overwriteBtn_clicked()
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

// добавить в любой файл
void task4::on_appendBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
	QFile file(fileName);
	QTextStream toFile(&file);

	if (!file.open(QFile::WriteOnly | QFile::Text | QFile::Append))
	{
		QMessageBox::warning(this, "Warning", "The file was not opened!");
	}
	else
	{
		QString text = ui.plainTextEdit->toPlainText();
		toFile << "\n" << text;
		file.flush();
		file.close();
	}
}

// записать массив в файл
void task4::arrToFile(Student* arr, int size) {
	if (size == 0) {
		QMessageBox::warning(this, "Warning", "First create an array!");
	}
	else {
		fileName = QFileDialog::getOpenFileName(this, "Select the file you wanna save to", "D:/labs_OAiP2/lab1", "Text File (*.txt)");
		QFile file(fileName);
		QTextStream toFile(&file);

		if (!file.open(QFile::WriteOnly | QFile::Text))
		{
			QMessageBox::warning(this, "Warning", "The file was not opened!");
		}
		else
		{
			QString text = "";
			for (int i = 0; i < size; i++) {
				text = arr[i].infoToString();
				toFile << text;
				file.flush();
			}
			file.close();
		}
	}

}

// сортировать массив из всех студентов
void task4::on_sortBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First create an array");
	}
	else {
		groupSort(students, 0, studentsSize - 1);
		int i = 0;
		int left, right;
		while (true) {
			left = i;
			while (i < studentsSize - 1 && students[i].getGrNum() == students[i + 1].getGrNum()) {
				i++;
			}
			right = i - 1;
			if (i == studentsSize - 1) {
				right = i;
			}
			fullNameSort(students, left, right);
			i++;
			if (i >= studentsSize - 1) {
				break;
			}
		}
	}
}

// создать массив из должников
void task4::on_debtorsBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First create an array");
	}
	else {
		debtors = findDebtors(debtors);
	}
}

// запись должников в файл
void task4::on_saveDebtorsBtn_clicked()
{
	if (debtorsSize == 0) {
		QMessageBox::warning(this, "Warning", "First create an array!");
	}
	else {
		arrToFile(debtors, debtorsSize);
	}
}

// создать массив студентов на отчисление
void task4::on_dropOutBtn_clicked()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First create an array");
	}
	else {
		dropOut = findDrop(dropOut);
	}
}

// запись студентов на отчисление в файл
void task4::on_saveDropOutBtn_clicked()
{
	if (dropOut == 0) {
		QMessageBox::warning(this, "Warning", "First create an array!");
	}
	else {
		arrToFile(dropOut, dropoutSize);
	}
}


void task4::on_searchByNameEdit_returnPressed()
{
	if (studentsSize == 0) {
		QMessageBox::warning(this, "Warning", "First add student");
	}
	else {
		QString name = ui.searchByNameEdit->text();
		groupSort(students, 0, studentsSize - 1);
		int i = 0;
		int left, right;
		while (true) {
			left = i;
			while (i < studentsSize - 1 && students[i].getGrNum() == students[i + 1].getGrNum()) {
				i++;
			}
			right = i - 1;
			if (i == studentsSize - 1) {
				right = i;
			}
			fullNameSort(students, left, right);
			i++;
			if (i >= studentsSize - 1) {
				break;
			}
		}
		int index = findStudent(students, studentsSize, name);

		if (index == -1) {
			ui.plainTextEdit_2->setPlainText("Such student does not exists");
		}
		else {
			QString info = students[index].infoToString();
			ui.plainTextEdit_2->setPlainText(info);
		}
	}
}

