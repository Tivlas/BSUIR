#pragma once
#include "Shape.h"	
class Hexagon : public Shape
{

public:
	Hexagon(double xCo,double yCo, double diameter);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;

};

