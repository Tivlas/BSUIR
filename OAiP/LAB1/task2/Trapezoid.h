#pragma once
#include "Shape.h"
class Trapezoid : public Shape
{
private:
	double x1, x2, x3, x4, y1, y2, y3, y4;
public:
	Trapezoid(double x1, double y1, double x2, double y2, double width);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;
};

