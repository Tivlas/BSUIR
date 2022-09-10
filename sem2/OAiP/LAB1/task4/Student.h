#pragma once
#include <QString>
class Student
{
private:
	int	grNum = 153501;// group number
	QString fullName = "Petrov Peter Perovich";

	int winSize = 0;
	int winterSum = 0;
	int sumSize = 0;
	int summerSum = 0;

	double winAvg = 0;
	double sumAvg = 0; // average mark
public:
	Student();
	~Student();
	QString getName() const { return fullName; };
	int getGrNum() const { return grNum; }
	int getWinSize() const { return winSize; }
	int getSumSize() const { return sumSize; }
	QString* setName() { return &fullName; }
	int* setGroupNum() { return &grNum; }
	int* setWinSize() { return &winSize; }
	int* setSumSize() { return &sumSize; }
	double* setWinAvg() { return &winAvg; }
	double* setSumAvg() { return &sumAvg; }
	int* setSummerSum() { return &summerSum; }
	int* setWinterSum() { return &winterSum; }
	int getSummerSum() const { return summerSum; }
	int getWinterSum() const { return winterSum; }


	void initialize(const Student&);


	void markSort(int*, int, int);
	bool findBadMark(int*, int);// winter session
	bool findTwoBadMarks(int*, int);
	QString infoToString();

	int* winMarks; // winter
	int* sumMarks; // summer
};

