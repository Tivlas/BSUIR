#pragma once
#include "Shape.h"
class Square : public Shape
{
public:
	Square();
	Square(double width);
	virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* opt, QWidget* widg) override;
	void wheelEvent(QGraphicsSceneWheelEvent*) override;
	void rotate(int) override;
	double perimeter() override;
	double area() override;
	QPointF findCentre() override;
};
