#pragma once
#include "Square.h"
class Rect : public Square
{
private:
	double height = 0;
public:
	Rect(double width,double height);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	double perimeter();
	double area();
};

