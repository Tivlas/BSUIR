#include "task2.h"
#include "stdafx.h"

task2::task2(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void task2::checkText(const QString& text)
{
	QChar ch;
	QChar open[] = { '[','{','(' };
	QChar close[] = { ']', '}', ')' };
	QStringList list = text.split(QLatin1Char('\n'));

	int closeIndex;
	int openIndex;

	for (size_t i = 0; i < list.size(); i++)
	{
		for (size_t j = 0; j < list[i].size();) {
			if (list[i][j] == '[' || list[i][j] == '{' || list[i][j] == '(') {
				ch = list[i][j];
				stack.push(new Node(ch));
				stack.peek()->setIndices(i + 1, j + 1);
				++j;
				continue;
			}

			if (list[i][j] == ']' || list[i][j] == '}' || list[i][j] == ')') {
				if (stack.empty()) {
					QString error = "Missing opening bracket for " + QString::number(i + 1) + " " +
						QString::number(j + 1) + " bracket";

					QMessageBox::critical(this, "Error", error);
					stack.clear();
					return;
				}
				ch = list[i][j];
				closeIndex = 0;

				for (int i = 0; i < 3; ++i) {
					if (ch == close[i]) {
						closeIndex = i;
						break;
					}
				}

				openIndex = 0;
				for (int i = 0; i < 3; ++i) {
					if (stack.peek()->getData() == open[i]) {
						openIndex = i;
						break;
					}
				}
				if (openIndex != closeIndex) {
					QString error = "Error in line " + QString::number(i + 1) + " " + QString::number(j + 1) + "\n" + list[i];
					QMessageBox::critical(this, "Error", error);
					stack.clear();
					return;
				}
				else {
					stack.pop();
				}
			}
			++j;
		}
	}
	if (!stack.empty()) {
		QString error = "Missing closing bracket for " + QString::number(stack.peek()->get_I_Index()) + " " +
			QString::number(stack.peek()->get_J_Index()) + " bracket";

		QMessageBox::critical(this, "Error", error);
		stack.clear();
		return;
	}
	QMessageBox::information(this, "Success", "No errors found");
}

void task2::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/BSUIR/OAiP/lab3", "Available extensions (*.txt *.cpp *.h)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		text = fromFile.readAll();
		ui.plainTextEdit->setPlainText(text);
		file.close();
		haveReadFromFile = true;
	}
}

void task2::on_checkBtn_clicked()
{
	if (haveReadFromFile) {
		text = ui.plainTextEdit->toPlainText();
		checkText(text);
		haveReadFromFile = false;
		return;
	}
	// else
	text = ui.plainTextEdit->toPlainText();
	checkText(text);
	return;
}

void task2::on_quitBtn_clicked() {
	QApplication::quit();
}

