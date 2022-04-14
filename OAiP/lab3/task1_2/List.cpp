#include "List.h"

void List::add(const Product& product) {
	if (!anyFreeCells)
	{
		next[productCount] = productCount + 1;
		products[productCount] = product;
		++productCount;
		return;
	}

	next[freeCells[freeCells.size() - 1] - 1] = freeCells[freeCells.size() - 1];
	products[freeCells[freeCells.size() - 1] - 1] = product;
	/*
	Например:
	next = {1,2,4,4,5}, значит третья ячейка(если считать с единицы) products свободна
	freeCells = {3};
	next[freeCells[freeCells.size() - 1] - 1] = freeCells[freeCells.size() - 1];
	freeCells[freeCells.size() - 1] - 1 = 3 - 1 = 2
	freeCells[freeCells.size() - 1] = 3;
	next[2] = 3;
	теперь next = {1,2,3,4,5};
	все ячейки заняты
	products[freeCells[freeCells.size() - 1] - 1] ~ products[2] = product;
	*/
	freeCells.pop_back();
	if (freeCells.size() == 0) {
		anyFreeCells = false;
	}
}

int List::find(QString name) {
	for (int i = next[0]; i < next.size();) {
		if (products[i-1].getName() == name) {
			return i;
		}
		i = next[i];
	}
	return -1; // если нет товара с таким названием
}

void List::remove(QString name) {
	int index = find(name);
	if (index != -1) {
		for (int i = 0; i < next.size(); ++i) {
			if (next[i] == index) {
				next[i] = next[index];
			}
		}
		anyFreeCells = true;
		freeCells.push_back(index);
	}
}

QString List::print() {
	QString result = "";
	for (int i = next[0]; i < next.size();) {
		result += products[i-1].getInfo();
		i = next[i];
	}
	return result;
}