#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_parser.h"
#include "Vector/Vector.h"
#include "Pair/MyPair.h"
#include <regex>

using SPair = MyPair<std::string, std::string>;
using PVector = Vector<SPair>;

class Parser : public QMainWindow
{
	Q_OBJECT

public:
	Parser(QWidget* parent = Q_NULLPTR);
	void findVarOfFundTypes(const QString& text, std::regex& fundTypesRegEx);
	void findStructsAndClasses(const QString& text);
	void findFunctionsPrototypes(const QString& text);
	void findVarChanges(const QString& text);
	void findLogicalErrors(const QString& text);

	void sortByType(PVector& v);
	void sortByName(PVector& v);

private slots:
	void on_quitBtn_clicked();
	void on_readBtn_clicked();
	void on_parseBtn_clicked();
	void on_codeTextEdit_textChanged();

private:
	Ui::ParserClass ui;
	PVector fundTypeVariables; // вектор пар для хранения переменных фундаментальных типов +
							  // (их имя + значение по умолчанию)
	PVector structsAndClasses; // вектор пар для хранения структур и классов + их имен
	PVector funcPrototypesV; // вектор пар для хранения прототипов функций
	Vector<std::string>  varChanges;
	Vector<std::string> logicalErrors;
	Vector<std::string> classStructNames;


	QString fileName;
	QString result = "";
	//std::regex fundTypesRegEx;
	std::regex structsClassesRegEx;
	std::regex functionsPrototypesRegEx;
	std::regex variablesChangesRegEx;
	// std::regex branchingDepthRegEx;
	std::regex logicalErrorsRegEx;


	size_t numberOfArrays = 0;
};
