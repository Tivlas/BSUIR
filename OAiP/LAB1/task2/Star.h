#pragma once
#include "Shape.h"
class Star : public Shape
{
public:
	Star(double xCo,double yCo, double radius, double diameter);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;

};

