#pragma once
#include "Shape.h"
class Triangle : public Shape
{
private:
	double x1, y1, x2, y2, x3, y3;
public:
	Triangle(double x1, double y1, double x2, double y2, double x3, double y3);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;
};

