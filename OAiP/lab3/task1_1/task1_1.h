#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task1_1.h"
#include "List.h"

class task1_1 : public QMainWindow
{
    Q_OBJECT

public:
    task1_1(QWidget *parent = Q_NULLPTR);
    bool dateValidation(const QString& date);
    bool dateCompare(int day1, int month1, int year1, int day2, int month2, int year2);
    int daysPassed(int day, int month,int year);
    int duration(int day1, int month1, int year1, int day2, int month2, int year2);
public slots:
    void on_quitBtn_clicked();
	void on_addBtn_clicked();
    void on_printBtn_clicked();
    void on_printEmployeesWithSpecificSurnameBtn_clicked();
	void on_printEmployeesWithSpecificDepNumberBtn_clicked();
	void on_fireBtn_clicked();
    void on_printExperienceBtn_clicked();

private:
    Ui::task1_1Class ui;
    QString file_name;
	List list; // list of employees
};
