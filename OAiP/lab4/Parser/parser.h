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
    QString fileName;
    QString result = "";
};
