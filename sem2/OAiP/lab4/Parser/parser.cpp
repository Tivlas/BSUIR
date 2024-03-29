﻿#include "parser.h"
#include "stdafx.h"
#include <algorithm>

Parser::Parser(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	structsClassesRegEx = ("(class|struct) +(\\w+)");
	variablesChangesRegEx = ("[A-Za-z0-9_\\.]+ *={1} *[\\{\\}A-Za-z0-9()+ \\.\\[\\]\\+\\(\\)&\\*/]+(;|,)");
	logicalErrorsRegEx = ("(const +bool.+;|while *\\([A-Za-z0-9 ]*\\)|(else)? *if *\\((true|false)?[0-9\\-\\.]*\\)|for *\\([^;]*; *(true|false) *;.*\\))");
	branchingDepthRegEx = ("(if *\\(.+\\) *\\{([^\\}]*)\\})( *else *\\{([^\\}]*)\\})*");
}

void Parser::findVarOfFundTypes(const QString& text, const std::regex& typesAndValuesRegEx)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, typesAndValuesRegEx))
	{
		std::string type = match[2].str() + match[3].str() + match[4].str();
		std::string value = match[5].str() + match[6].str();
		bool newFound = false;// checking for dynamic array
		for (size_t j = 0; j < value.size(); ++j) {
			if (value[j] == 'n' && value[j + 1] == 'e' && value[j + 2] == 'w' && value[j + 3] == ' ')
			{
				newFound = true;
				break;
			}
		}
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
			bool EqualOperatorFound = false;
			for (size_t j = 0; j < var.size(); ++j)
			{
				if (var[j] == '=') {
					EqualOperatorFound = true;
				}
				else if (var[j] == '*' && !EqualOperatorFound)
				{
					temp_type += "*";
					var.erase(j, 1);
				}
				else if (var[j] == '&' && !EqualOperatorFound)
				{
					temp_type += "&";
					var.erase(j, 1);
				}
			}
			if (!newFound)
			{
				if (var.find(']') != std::string::npos) {
					size_t index = 0;
					while (var[index] != ']') {
						++index;
					}
					if (var[index + 1] == '=' || var[index+1] ==';')
					{
						temp_type += " array ";
						++numberOfArrays;
					}
				}
			}
			else if (newFound) {
				temp_type += " (dynamic array) ";
				for (size_t j = 0; j < var.size(); ++j) {
					if (var[j] == 'n' && var[j + 1] == 'e' && var[j + 2] == 'w')
					{
						var.insert(j + 3, " ");
					}
				}
				++numberOfArrays;
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
	while (std::regex_search(input, match, structsClassesRegEx))
	{
		std::string type = match[1].str();
		std::string name = match[2].str();
		structsAndClasses.push_back(SPair(type, name));
		classStructNames.push_back(name);
		input = match.suffix().str();
	}
}


void Parser::findFunctionsPrototypes(const QString& text, const std::regex& functionsPrototypesRegEx)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, functionsPrototypesRegEx))
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
	while (std::regex_search(input, match, variablesChangesRegEx))
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
		change.erase(std::remove(change.begin(), change.end(), ' '), change.end());
		
		varChanges.push_back(change);
		input = match.suffix().str();
	}
}


void Parser::findLogicalErrors(const QString& text)
{
	std::string input = text.toStdString();
	std::smatch match;
	while (std::regex_search(input, match, logicalErrorsRegEx))
	{
		std::string error = match[0].str();
		error.erase(std::remove(error.begin(), error.end(), ' '), error.end());
		logicalErrors.push_back(error);
		input = match.suffix().str();
	}
}


int Parser::findBranchingDepth(const QString& text, int cur = 0)
{
	std::string input = text.toStdString();
	std::smatch match;
	int depth = cur;
	while (std::regex_search(input, match, branchingDepthRegEx)) {
		if (match[2].matched) {
			depth = std::max(depth, findBranchingDepth(QString(match[2].str().c_str()) + "}", cur + 1));
		}
		if (match[4].matched) {
			depth = std::max(depth, findBranchingDepth(QString(match[4].str().c_str()) + "}", cur + 1));
		}
		input = match.suffix();
	}
	return depth;
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


void Parser::on_parseBtn_clicked()
{
	ui.resultTextEdit->clear();
	result = "";
	ui.parseBtn->setEnabled(false);
	QString text = ui.codeTextEdit->toPlainText();
	QTextCharFormat notInBold = ui.resultTextEdit->currentCharFormat(), inBold;
	inBold.setFontWeight(QFont::Bold);

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

	int i = 0;
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
		result = QString::fromStdString("Тип: " + structsAndClasses[i].first) + ", количество = " + QString::number(count);
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
	//++++++++++++Переменные++++++++++++
	//++++++++++++++++++++++++++++++++++
	std::string const_unsigned = "((const *)?(signed *|unsigned *)? *(";
	std::string types = "QString|std::string|std::vector\\<\\w+\\>|size_t|bool|char|int|short|void|long long|float|long double|double|long|auto|";
	for (size_t j = 0; j < classStructNames.size(); ++j) {
		types += classStructNames[j] + "|";
	}
	types.pop_back();
	std::string regularExForTypes = const_unsigned + types + ")(\\**&* +\\**&*)( *[:A-Za-z_;\\.0-9]+\\,?[:'\\+\\*&A-Za-z\\.\\s,\\[\\]0-9=_\\{\\}]*;))";
	std::regex typesAndValuesRegEx(regularExForTypes);

	findVarOfFundTypes(text, typesAndValuesRegEx);
	sortByType(fundTypeVariables);
	result = "ПЕРЕМЕННЫЕ:";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	i = 0;
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
		result = QString::fromStdString("Тип: " + fundTypeVariables[i].first) + ", количество переменных = " + QString::number(count);
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
	//++++++++++++Функции+++++++++++++++
	//++++++++++++++++++++++++++++++++++

	std::string strRegExForFuncPrototypes = const_unsigned + types + ")(\\**&* +\\**&*)( *[:A-Za-z0-9_=\\+\\-]+[(][:A-Za-z_ 0-9=&\\*,\\(\\)]*[)]))";
	std::regex functionsPrototypesRegEx(strRegExForFuncPrototypes);
	findFunctionsPrototypes(text, functionsPrototypesRegEx);
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
	result = "СТРОКИ С ИЗМЕНЕНИЕМ ПЕРЕМЕННЫХ:";
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

	//++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++ Логические ошибки +++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++
	findLogicalErrors(text);
	result = "ЛОГИЧЕСКИЕ ОШИБКИ(или потенциальные логические ошибки):";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";

	for (size_t i = 0; i < logicalErrors.size(); i++)
	{
		result += QString::fromStdString(logicalErrors[i]) + "\n";
	}

	ui.resultTextEdit->append(result);
	result = "";

	//++++++++++++++++++++++++++++++++++++++++++++
	//++++++++++  Глубина ветвления ++++++++++++++
	//++++++++++++++++++++++++++++++++++++++++++++
	result = "МАКСИМАЛЬНАЯ ГЛУБИНА ВЕТВЛЕНИЯ:";
	ui.resultTextEdit->setCurrentCharFormat(inBold);
	ui.resultTextEdit->append(result);
	ui.resultTextEdit->setCurrentCharFormat(notInBold);
	result = "";
	int depth = findBranchingDepth(text);
	result = QString::number(depth);
	ui.resultTextEdit->append(result);
	result = "";

	fundTypeVariables.clear();
	structsAndClasses.clear();
	funcPrototypesV.clear();
	varChanges.clear();
	logicalErrors.clear();
	classStructNames.clear();
}


void Parser::on_codeTextEdit_textChanged()
{
	ui.parseBtn->setEnabled(true);
}


void Parser::on_readBtn_clicked()
{
	fileName = QFileDialog::getOpenFileName(this, "Open your file", "D:/BSUIR/OAiP/lab4", "Text File (*.cpp *.h)");
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


void Parser::on_quitBtn_clicked() {
	QApplication::quit();
}
