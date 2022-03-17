#pragma once
#include "Shape.h"
#define PI 3.14
class Circle : public Shape
{	
public:
	Circle(double x = 0, double y = 0, double diameter = 0);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;
};

