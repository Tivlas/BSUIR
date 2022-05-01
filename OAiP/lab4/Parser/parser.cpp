#include "parser.h"
#include "stdafx.h"

Parser::Parser(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
}

void Parser::findVarOfFundTypes(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	std::regex fundTypesRegEx("((const *)?(signed *|unsigned *)?(size_t|bool|char|int|short|void|(long)? *long|float|(long)? *double|auto)(\\**&* +\\**&*))( *[^\\(^\\)^;]+\\,?.*?;)");

	while (std::regex_search(input, match, fundTypesRegEx))
	{
		std::string type = match[2].str() + match[3].str() + match[4].str() + match[7].str();
		std::string value = match[8].str();
		value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
		for (int i = 0; i < value.size(); i++)
		{
			if (value[i] == '{') {
				i++;
				while (value[i] != '}') {
					i++;
				}
			}
			if (value[i] == '\"') {
				i++;
				while (value[i] != '\"') {
					i++;
				}

			}
			if (value[i] == ',')
			{
				value[i] = ' ';
			}
		}
		QString valueQ = QString::fromStdString(value);
		QStringList list = valueQ.split(QLatin1Char(' '), Qt::SkipEmptyParts);
		for (size_t i = 0; i < list.size(); ++i) {
			std::string var = list[i].toStdString();
			std::string temp_type = type;
			bool newFound = false;
			for (size_t j = 0; j < var.size(); ++j) {
				if (var[j] == 'n' && var[j + 1] == 'e' && var[j + 2] == 'w')
				{
					var.insert(j + 3, " ");
					newFound = true;
					break;
				}
			}

			if (!newFound)
			{
				if (var.find('[') != std::string::npos) {
					temp_type += "array ";
				}
			}
			if (var.back() != ';') { var += ';'; }
			fundTypeVariables.push_back(SPair(temp_type, var));
		}
		input = match.suffix().str();
	}
}


void Parser::sortByType(PVector& v)
{
	SPair temp;
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v.size() - i - 1; j++)
		{
			if (v[j].first > v[j + 1].first)
			{
				temp = v[j];
				v[j] = v[j + 1];
				v[j + 1] = temp;
			}
		}
	}
}

void Parser::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", /*"D:/BSUIR/OAiP/"*/ "D:/Лабы по проге/1 сем/4 lab/task_10", "Text File (*.txt *.cpp)");
	QFile file(fileName);
	QTextStream fromFile(&file);

	if (!file.open(QFile::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "Warning", "The file was not opened or it's empty!");
	}
	else
	{
		QString text = fromFile.readAll();
		ui.codeTextEdit->setPlainText(text);
		file.close();
	}
}

void Parser::on_parseBtn_clicked()
{
	ui.resultTextEdit->clear();
	result = "";
	ui.parseBtn->setEnabled(false);
	QString text = ui.codeTextEdit->toPlainText();
	findVarOfFundTypes(text);
	sortByType(fundTypeVariables);
	QTextCharFormat notInBold = ui.resultTextEdit->currentCharFormat(), inBold;
	inBold.setFontWeight(QFont::Bold);
	result += "ПЕРЕМЕННЫЕ:\n";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->appendPlainText(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);


	int i = 0;
	while (i!=fundTypeVariables.size())
	{
		int count = 0;
		int first = i;
		std::string t1 = fundTypeVariables[i].first;
		std::string t2 = fundTypeVariables[i+1].first;
		std::string tt1 = fundTypeVariables[i].second;
		std::string tt2 = fundTypeVariables[i + 1].second;
		while (fundTypeVariables[i].first == fundTypeVariables[i + 1].first && i < fundTypeVariables.size() - 1) {
			count++;
			i++;
		}
		count++;

		ui.resultTextEdit->setCurrentCharFormat(inBold);
		result += QString::fromStdString("ТИП: " + fundTypeVariables[i-1].first) + ", КОЛИЧЕСТВО ПЕРЕМЕННЫХ = " + QString::number(count) + "\n";
		ui.resultTextEdit->appendPlainText(result);
		ui.resultTextEdit->setCurrentCharFormat(notInBold);
		for (size_t j = first; j <= i; j++)
		{
			result += QString::fromStdString(fundTypeVariables[j].second) + "\n";
		}
		
		result += "\n";
		++i;
	}
	

	fundTypeVariables.clear();
	ui.resultTextEdit->setPlainText(result);
}

void Parser::on_codeTextEdit_textChanged()
{
	ui.parseBtn->setEnabled(true);
}

void Parser::on_quitBtn_clicked() {
	QApplication::quit();
}
