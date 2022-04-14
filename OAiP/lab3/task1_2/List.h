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
	int productCount = 0;
public:
	List(){}
	~List(){}
	void add(const Product& product);
	int find(QString name);
	void remove(QString name);
	//Product get(int index);
	int getSize();
	QString print();
	
};

