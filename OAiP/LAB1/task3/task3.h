#pragma once

#include <QtWidgets/QMainWindow>
#include <QString>
#include <cmath>
#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include "ui_task3.h"
#include "Date.h"


class Date
{
	
private:
	int m_day = 5, m_month = 10, m_year = 2003;
    int leapYear[12] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
	int notLeapYear[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
public:
	
	Date(int m_day = 5, int m_month = 10, int m_year = 2003);

	Date NextDay() const;

    Date PreviousDay() const;

	bool isLeap() const;

	bool DaysInMonth(int month) const; // 30 or 31

	int daysPassed() const; // from the beginnibg f the year

    short WeekNumber() const;

    int DaysTillBirthday(Date&) const;
   
    int Duration(const Date&) const;

	bool dateCompare(const Date&) const;

	QTableWidgetItem* dateItem() const;

	QString dateToString() const;

	int getYear()const { return m_year; }
   
public:
	void setDay(int day) { this->m_day = day; }
	void setMonth(int month) { this->m_month = month; }
	void setYear(int year) { this->m_year = year; }

};


class task3 : public QMainWindow
{
	Q_OBJECT

public:
	task3(QWidget* parent = Q_NULLPTR);

public slots:
	void on_getBtn_clicked();
	void on_nextBtn_clicked();
	void on_previousBtn_clicked();
	void on_weekBtn_clicked();
	void on_leapBtn_clicked();
	void on_durationBtn_clicked();
	void on_birthdayEdit_returnPressed();
	void on_quitBtn_clicked();
	void on_addEdit_returnPressed();
	void on_saveBtn_clicked();

	void on_readBtn_clicked();
	void on_writeBtn_clicked();



private:
	Ui::task3Class ui;
	Date* dates = new Date[1];
	QString fileName;
	int size = 0;
};