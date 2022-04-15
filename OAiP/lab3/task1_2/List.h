#pragma once
#include "Array.h"
#include "Product.h"
#include <QString>
		
class List
{
private:
	Array<Product> products;
	Array<int> next;
	Array<int> freeCells;// массив хранит индексы забытых ячеек массива(после удаления элемента)
	bool anyFreeCells = false;// есть ли в массиве свободные ячейки
	int productCount = 0;// считает количество ячеек, которые были использованы хотя бы один раз
	int actualSize = 0; // считает количество различных по имени товаров
public:
	List(){}
	~List(){}
	void add(const Product& product);
	int find(QString name);
	void remove(QString name);
	void removeAll(QString name);
	void clear();

	int getActualSize() const {
		return actualSize;
	}
	
	QString print();
	QString printByProductName(QString name);
	QString printProductsWithLowerPrice(int price); // выводит товары с меньшей ценой
	QString printByStorageTime(int storageDays); // выводит товары которые были на складе больше введенного количества дней
	
};

bool dateCompare(int day1, int month1, int year1, int day2, int month2, int year2);

int daysPassed(int day, int month, int year);

int duration(int day1, int month1, int year1, int day2, int month2, int year2);