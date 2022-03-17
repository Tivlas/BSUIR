#pragma once
#include <QPainter>
#include <QGraphicsItem>
#include <QGraphicsSceneWheelEvent>
#include <QWidget>
#include <cmath>

class Shape : public  QGraphicsItem
{
protected:
	double scl = 1; // scale
	double diameter = 100, radius = 50;
	double ang = 0; // angle
	double side = 0;
	double width = 0;// rectangle, square, trapezoid
	double xCo = 0, yCo = 0; //coordinates of the centre
	int num = 5; // star vertices
public:
	double* changeDiam() { return &diameter; }
	double* changeWidth() { return &width; }
	int* changeNum() { return &num; }
	double* changeXC() { return &xCo; }
	double* changeYC() { return &yCo; }


	virtual QRectF boundingRect() const { return QRectF(-1600, -1600, 2400, 2400); }

	virtual void paint(QPainter*, const QStyleOptionGraphicsItem* = 0, QWidget* = 0) = 0;

	virtual void wheelEvent(QGraphicsSceneWheelEvent*) = 0;

    virtual double perimeter() = 0;

	virtual double area() = 0;

	virtual void rotate(int) = 0;

	virtual QPointF findCentre() = 0;
};

