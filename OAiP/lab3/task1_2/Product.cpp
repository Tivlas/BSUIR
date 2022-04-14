#include "Product.h"

QString Product::getInfo()
{
	QString result;
	result = "Product: " + name + "\n" + "Price: " + QString::number(price) + "\n\n";
	return result;
}
