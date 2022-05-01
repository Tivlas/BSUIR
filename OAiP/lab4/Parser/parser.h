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
    Parser(QWidget *parent = Q_NULLPTR);
    void findVarOfFundTypes(const QString& text);
    void findStructsAndClasses(const QString& text);
    void sortByType(PVector& v);

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
    QString fileName;
    QString result = "";
    std::regex fundTypesRegEx;
    std::regex structs_classes;
    size_t numberOfArrays = 0;
};
