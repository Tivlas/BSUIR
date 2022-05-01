#include "parser.h"
#include "stdafx.h"

Parser::Parser(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	fundTypesRegEx = ("((const *)?(signed *|unsigned *)? *(std::string|std::vector\\<\\w+\\>|size_t|bool|char|int|short|void|long long|float|long double|double|long|auto)(\\**&* +\\**&*))( *[A-Za-z_;]+\\,?[A-Za-z\\. ,\\[\\]0-9=_\\{\\}]*;)");
	structsClasses = ("(class|struct) +(\\w+)");
	functionsPrototypes = ("((const *)?(signed *|unsigned *)? *(std::string|std::vector\\<\\w+\\>|size_t|bool|char|int|short|void|long long|float|long double|double|long|auto)(\\**&* +\\**&*))( *[A-Za-z0-9_]+[(][A-Za-z_ 0-9=]*[)])");
	variablesChanges = ("[A-Za-z0-9_]+ *={1} *[\\{\\}A-Za-z0-9()+ \\.\\[\\],]+;");
}

void Parser::findVarOfFundTypes(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, fundTypesRegEx))
	{
		std::string type = match[2].str() + match[3].str() + match[4].str() + match[5].str();
		std::string value = match[6].str();
		value.erase(std::remove(value.begin(), value.end(), ' '), value.end());
		for (int i = 0; i < value.size(); i++)
		{
			if (value[i] == '{') {
				i++;
				while (value[i] != '}') {
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
				if (var.find(']') != std::string::npos) {
					size_t index = 0;
					while (var[index] != ']') {
						++index;
					}
					if (var[index + 1] == '=')
					{
						temp_type += "array ";
						++numberOfArrays;
					}
				}
			}
			if (var.back() != ';') { var += ';'; }
			fundTypeVariables.push_back(SPair(temp_type, var));
		}
		input = match.suffix().str();
	}
}

void Parser::findStructsAndClasses(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, structsClasses))
	{
		std::string type = match[1].str();
		std::string name = match[2].str();
		structsAndClasses.push_back(SPair(type, name));
		input = match.suffix().str();
	}
}

void Parser::findFunctionsPrototypes(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, functionsPrototypes))
	{
		std::string type = match[2].str() + match[3].str() + match[4].str() + match[5].str();
		std::string name = match[6].str();
		funcPrototypesV.push_back(SPair(type, name));
		input = match.suffix().str();
	}
}

void Parser::findVarChanges(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, variablesChanges))
	{
		std::string change = match[0].str();
		size_t openBracketCounter = 0;
		size_t closeBracketCounter = 0;
		for (size_t i = 0; i < change.size(); ++i) {
			if (change[i] == '(') {
				openBracketCounter++;
			}
			if (change[i] == ')') {
				closeBracketCounter++;
			}
		}
		if (openBracketCounter != closeBracketCounter) {
			input = match.suffix().str();
			continue;
		}
		varChanges.push_back(change);
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

void Parser::sortByName(PVector& v)
{
	SPair temp;
	for (int i = 0; i < v.size(); i++)
	{
		for (int j = 0; j < v.size() - i - 1; j++)
		{
			if (v[j].second > v[j + 1].second)
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
	fileName = QFileDialog::getOpenFileName(this, "Open your file", /*"D:/BSUIR/OAiP/"*/ "D:/Tests/C++_Tests/Tests/RegExTest", "Text File (*.txt *.cpp)");
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

	//++++++++++++++++++++++++++++++++++
	//++++++++++++Переменные++++++++++++
	//++++++++++++++++++++++++++++++++++
	findVarOfFundTypes(text);
	sortByType(fundTypeVariables);
	QTextCharFormat notInBold = ui.resultTextEdit->currentCharFormat(), inBold;
	inBold.setFontWeight(QFont::Bold);
	result = "ПЕРЕМЕННЫЕ:";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	int i = 0;
	while (i != fundTypeVariables.size())
	{
		int count = 0;
		int first = i;
		while (fundTypeVariables[i].first == fundTypeVariables[i + 1].first && i < fundTypeVariables.size() - 1) {
			count++;
			i++;
		}
		count++;

		ui.resultTextEdit->setCurrentCharFormat(inBold);
		result = QString::fromStdString("Тип: " + fundTypeVariables[i].first) + ", Количество переменных = " + QString::number(count);
		ui.resultTextEdit->append(result);
		ui.resultTextEdit->setCurrentCharFormat(notInBold);
		result = "";
		for (size_t j = first; j <= i; j++)
		{
			result += QString::fromStdString(fundTypeVariables[j].second) + "\n";
		}

		ui.resultTextEdit->append(result);
		result = "";
		++i;
	}

	//++++++++++++++++++++++++++++++++++
	//+++++++ Структуры и классы +++++++
	//++++++++++++++++++++++++++++++++++
	findStructsAndClasses(text);
	sortByType(structsAndClasses);
	result = "СТРУКТУРЫ И КЛАССЫ:";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	i = 0;
	while (i != structsAndClasses.size())
	{
		int count = 0;
		int first = i;
		while (structsAndClasses[i].first == structsAndClasses[i + 1].first && i < structsAndClasses.size() - 1) {
			count++;
			i++;
		}
		count++;

		ui.resultTextEdit->setCurrentCharFormat(inBold);
		result = QString::fromStdString("Тип: " + structsAndClasses[i].first) + ", Количество переменных = " + QString::number(count);
		ui.resultTextEdit->append(result);
		ui.resultTextEdit->setCurrentCharFormat(notInBold);
		result = "";
		for (size_t j = first; j <= i; j++)
		{
			result += QString::fromStdString(structsAndClasses[j].second) + "\n";
		}

		ui.resultTextEdit->append(result);
		result = "";
		++i;
	}

	//++++++++++++++++++++++++++++++++++
	//++++++++++++Функции+++++++++++++++
	//++++++++++++++++++++++++++++++++++
	findFunctionsPrototypes(text);
	sortByType(funcPrototypesV);
	sortByName(funcPrototypesV);
	result = "ПРОТОТИПЫ ФУНКЦИЙ:";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	for (size_t i = 0; i < funcPrototypesV.size(); i++)
	{
		result += QString::fromStdString(funcPrototypesV[i].first) + QString::fromStdString(funcPrototypesV[i].second) + "\n";
	}
	ui.resultTextEdit->append(result);
	result = "";

	//++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++ Изменения переменных ++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++
	findVarChanges(text);
	result = "ИЗМЕНЕНИЯ ПЕРЕМЕННЫХ(включая инициализацию):";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	for (size_t i = 0; i < varChanges.size(); i++)
	{
		result += QString::fromStdString(varChanges[i]) + "\n";
	}
	ui.resultTextEdit->append(result);
	result = "";


	fundTypeVariables.clear();
	structsAndClasses.clear();
	funcPrototypesV.clear();
	varChanges.clear();
	ui.resultTextEdit->append(result);
}

void Parser::on_codeTextEdit_textChanged()
{
	ui.parseBtn->setEnabled(true);
}

void Parser::on_quitBtn_clicked() {
	QApplication::quit();
}
