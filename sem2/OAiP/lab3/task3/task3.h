#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task3.h"
#include "Stack.h"

class task3 : public QMainWindow
{
	Q_OBJECT

public:
	task3(QWidget* parent = Q_NULLPTR);
	int precedence(QChar op);
	bool isOperator(QChar op);
	void infixToPostfix(const QString& infix, QString& postfix);
	double performOperation(double a,double b,QChar op);
	double calculateInRPN(QString& postfix,double a,double b,double c, double d, double e);
	bool infixValidation(const QString& infix);
	bool variableValidation(const QString& var);
	bool checkBrackets(const QString&);
public slots:
	void on_quitBtn_clicked();
	void on_calcBtn_clicked();
private:
	Ui::task3Class ui;
	QString infix = "";
	QString postfix = "";
};
