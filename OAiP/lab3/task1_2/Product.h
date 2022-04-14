#pragma once
#include <QString>
class Product
{
private:
	friend class List;
	QString name = "";
	int price = 0;
public:
	Product(){}
	Product(QString name, int price): name(name), price(price) {}
	
	QString getName() {
		return name;
	}
	int getPrice() {
		return price;
	}
	void setName(QString name) {
		this->name = name;
	}
	void setPrice(int price) {
		this->price = price;
	}

	QString getInfo();
};


