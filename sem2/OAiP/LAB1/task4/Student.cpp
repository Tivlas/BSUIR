#include "Student.h"

Student::Student()
{
	this->fullName = "Nothing";
	this->grNum = 0;
	this->sumAvg = 0;
	this->winAvg = 0;
	this->sumMarks = new int[1];
	sumMarks[0] = 0;
	this->winMarks = new int[1];
	winMarks[0] = 0;
}

Student::~Student()
{
	delete[] sumMarks;
	delete[] winMarks;
}

bool Student::findBadMark(int* arr, int size)
{
	short counter = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] < 4) {
			counter++;
		}
	}
	if (counter == 1) {
		return true;
	}
	else {
		return false;
	}
}

bool Student::findTwoBadMarks(int* arr, int size)
{
	short counter = 0;
	for (int i = 0; i < size; i++) {
		if (arr[i] < 4) {
			counter++;
		}
	}
	if (counter == 2) {
		return true;
	}
	else {
		return false;
	}
}

QString Student::infoToString()
{
	QString num = QString::number(this->grNum) + '\n';
	QString name = this->fullName + '\n';


	QString winter = "";
	bool done = false;
	for (int i = 0; i < this->getWinSize(); i++) {
		done = true;
		winter += QString::number(this->winMarks[i]);
		if (i != this->getWinSize() - 1) {
			winter += ", ";
		}
	}
	if (!done) {
		winter += "empty";
	}
	winter += '\n';

	done = false;
	QString summer = "";
	for (int i = 0; i < this->getSumSize(); i++) {
		done = true;
		summer += QString::number(this->sumMarks[i]);
		if (i != this->getSumSize() - 1) {
			summer += ", ";
		}
	}
	if (!done) {
		summer += "empty";
	}
	summer += '\n';

	QString winterAvg = QString::number(this->winAvg) + '\n';
	QString summerAvg = QString::number(this->sumAvg) + "\n\n";

	QString text = num + name + winter + winterAvg + summer + summerAvg;
	return text;
}

void Student::initialize(const Student& other)
{
	this->fullName = other.fullName;
	this->grNum = other.grNum;
	this->winSize = other.winSize;
	this->winAvg = other.winAvg;
	this->sumSize = other.sumSize;
	this->sumAvg = other.sumAvg;

	this->winMarks = new int[other.winSize];
	for (int i = 0; i < other.winSize; i++) {
		this->winMarks[i] = other.winMarks[i];
	}

	this->sumMarks = new int[other.sumSize];
	for (int i = 0; i < other.sumSize; i++) {
		this->sumMarks[i] = other.sumMarks[i];
	}
}

void Student::markSort(int* a, int left, int right)
{
	int l = left, r = right;
	int temp, mid = a[(r + l) / 2];
	while (l < r) {
		while (a[l] < mid) {
			l++;
		}
		while (a[r] > mid) {
			r--;
		}
		if (l <= r) {
			temp = a[l]; a[l] = a[r]; a[r] = temp;
			l++; r--;
		}
		if (left < l) {
			markSort(a, left, r);
		}
		if (r < right) {
			markSort(a, l, right);
		}
	}
}