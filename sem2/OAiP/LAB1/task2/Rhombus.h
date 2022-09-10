#pragma once
#include "Shape.h"
class Rhombus : public Shape
{
private:
	double x1, x2, y1, y2, x3, x4, y3, y4;
	double diag1, diag2;
public:
	Rhombus(double diag1,double diag2);
	void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	double perimeter() override;
	double area() override;
	void rotate(int) override;
	QPointF findCentre() override;
};

