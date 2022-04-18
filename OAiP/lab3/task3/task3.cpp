#include "task3.h"
#include "stdafx.h"
#include "Stack.h"

task3::task3(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedHeight(499);
	this->setFixedWidth(849);
	ui.tableWidget->setHorizontalHeaderLabels(QStringList() << "Expression" << "a" << "b" << "c" << "d" << "e" << "Result");
	ui.tableWidget->setVerticalHeaderLabels(QStringList() << "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10" << "11" << "12" << "13" << "14" << "15");
}

int task3::precedence(QChar op)
{
	if (op == '+' || op == '-') {
		return 1;
	}
	else if (op == '*' || op == '/') {
		return 2;
	}
	return -1;
}

bool task3::isOperator(QChar op)
{
	return (op == '+' || op == '-' || op == '*' || op == '/');
}

bool task3::infixValidation(const QString& infix)
{
	int operatorsCounter = 0;
	int variablesCounter = 0;
	if (infix.size() == 0) {
		QMessageBox::warning(this, "Warning", "String must not be empty!");
		return false;
	}
	for (int i = 0; i < infix.size(); ++i) {
		if (!isOperator(infix[i]) && infix[i] != '(' && infix[i] != ')' && infix[i] != 'a' &&
			infix[i] != 'b' && infix[i] != 'c' && infix[i] != 'd' && infix[i] != 'e') {
			QMessageBox::warning(this, "Warning", "The string must contain only following characters:\n+ - * / ( ) a b c d e.\nMust not contain spaces");
			return false;
		}
		if (isOperator(infix[i])) {
			if (i == 0) {
				QMessageBox::warning(this, "Warning", "The string must not start with an operator");
				return false;
			}
			if (i == infix.size() - 1) {
				QMessageBox::warning(this, "Warning", "The string must not end with an operator");
				return false;
			}
			if (isOperator(infix[i - 1]) || isOperator(infix[i + 1])) {
				QMessageBox::warning(this, "Warning", "Operator must be followed by two operands");
				return false;
			}
			++operatorsCounter;
		}
		if (infix[i] == 'a' || infix[i] == 'b' || infix[i] == 'c' || infix[i] == 'd' || infix[i] == 'e') {
			++variablesCounter;
		}
	}
	if (variablesCounter - operatorsCounter != 1) {
		QMessageBox::critical(this, "Error", "The number of variables must differ from the number of\n operators by 1!");
		return false;
	}
	if (!checkBrackets(infix)) {
		QMessageBox::warning(this, "Warning", "Brackets are placed incorrectly!");
		return false;
	}
	return true;
}

bool task3::variableValidation(const QString& var)
{
	std::string temp = var.toStdString();
	if (var == "") {
		QMessageBox::warning(this, "Warning", "Variable must not be empty");
		return false;
	}
	if (var[0] == '.') {
		QMessageBox::warning(this, "Warning", "Variable must not start with a dot");
		return false;
	}

	size_t dotCounter = 0;
	for (int i = 0; i < var.size(); ++i) {
		if ((temp[i] < 48 || temp[i]>57) && temp[i] != '.') {
			QMessageBox::warning(this, "Warning", "Variable must contain only numbers and dot");
			return false;
		}
		if (temp[i] == '.') {
			dotCounter++;
			if (dotCounter > 1) {
				QMessageBox::warning(this, "Warning", "Variable must contain only one dot");
				return false;
			}
		}
	}
	return true;
}

bool task3::checkBrackets(const QString& text)
{
	Stack<QChar> stack;
	QChar ch;
	QStringList list = text.split(QLatin1Char('\n'));

	for (size_t i = 0; i < list.size(); i++)
	{
		for (size_t j = 0; j < list[i].size();) {
			if (list[i][j] == '(') {
				ch = list[i][j];
				stack.push((ch));
				++j;
				continue;
			}

			if (list[i][j] == ')') {
				if (stack.empty()) {
					return false;
				}
				stack.pop();
			}
			++j;
		}
	}
	if (!stack.empty()) {
		return false;
	}
	return true;
}

void task3::infixToPostfix(const QString& infix, QString& postfix)
{
	Stack<QChar> s; // Стек для операторов
	for (int i = 0; i < infix.length(); i++) {
		if (infix[i] == '(') {
			s.push(infix[i]);
		}
		else if (infix[i] == ')') {
			while ((s.peek() != '(') && (!s.empty())) {
				postfix += s.peek();
				s.pop();
			}
			s.pop();
		}
		else if (infix[i] == '+' || infix[i] == '-' || infix[i] == '*' || infix[i] == '/') {
			if (s.empty()) {
				s.push(infix[i]);
			}
			else {
				if (precedence(infix[i]) > precedence(s.peek())) {
					s.push(infix[i]);
				}
				else {
					while (!s.empty() && precedence(infix[i]) <= precedence(s.peek())) {
						postfix += s.peek();
						s.pop();
					}
					s.push(infix[i]);
				}
			}
		}
		else {
			postfix += infix[i];
		}
	}
	while (!s.empty()) {
		postfix += s.peek();
		s.pop();
	}
}

double task3::performOperation(double a, double b, QChar op)
{
	if (op == '+') {
		return a + b;
	}
	else if (op == '-') {
		return a - b;
	}
	else if (op == '*') {
		return a * b;
	}
	else if (op == '/') {
		if (b == 0) {
			throw QString("Division by zero");
		}
		return a / b;
	}
	return 0.0;
}

double task3::calculateInRPN(QString& postfix, double a = 0, double b = 0, double c = 0, double d = 0, double e = 0)
{
	Stack<double> s;
	for (int i = 0; i < postfix.length(); i++) {
		if (isOperator(postfix[i])) {
			double b = s.peek();
			s.pop();
			double a = s.peek();
			s.pop();
			s.push(performOperation(a, b, postfix[i]));
		}
		else {
			if (postfix[i] == 'a') {
				s.push(a);
			}
			else if (postfix[i] == 'b') {
				s.push(b);
			}
			else if (postfix[i] == 'c') {
				s.push(c);
			}
			else if (postfix[i] == 'd') {
				s.push(d);
			}
			else if (postfix[i] == 'e') {
				s.push(e);
			}
		}
	}
	return s.peek();
}

void task3::on_quitBtn_clicked() {
	QApplication::quit();
}

void task3::on_calcBtn_clicked() {
	for (int i = 0; i < 15; ++i) {
		infix = ui.tableWidget->item(i, 0)->text();
		if (!infixValidation(infix)) {
			QString err = "Incorrect input in row " + QString::number(i + 1) + " column 1";
			QMessageBox::critical(this, "Error", err);
			return;
		}
		infixToPostfix(infix, postfix);

		if (!variableValidation(ui.tableWidget->item(i, 1)->text())) {
			return;
		}
		double a = ui.tableWidget->item(i, 1)->text().toDouble();

		if (!variableValidation(ui.tableWidget->item(i, 2)->text())) {
			return;
		}
		double b = ui.tableWidget->item(i, 2)->text().toDouble();

		if (!variableValidation(ui.tableWidget->item(i, 3)->text())) {
			return;
		}
		double c = ui.tableWidget->item(i, 3)->text().toDouble();

		if (!variableValidation(ui.tableWidget->item(i, 4)->text())) {
			return;
		}
		double d = ui.tableWidget->item(i, 4)->text().toDouble();

		if (!variableValidation(ui.tableWidget->item(i, 5)->text())) {
			return;
		}
		double e = ui.tableWidget->item(i, 5)->text().toDouble();

		double result;
		try {
			result = calculateInRPN(postfix, a, b, c, d, e);
		}
		catch (QString& s) {
			ui.tableWidget->setItem(i, 6, new QTableWidgetItem(s));
			QMessageBox::critical(this, "Error", s);
			infix = "";
			postfix = "";
			break;
		}
		ui.tableWidget->setItem(i, 6, new QTableWidgetItem(QString::number(result)));
		infix = "";
		postfix = "";
	}
}