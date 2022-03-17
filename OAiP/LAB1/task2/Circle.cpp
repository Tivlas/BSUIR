#include "Circle.h"

Circle::Circle(double x, double y, double diameter)
{
	setFlag(ItemIsMovable);
	this->diameter = diameter;
	xCo = x;
	yCo = y;
}

void Circle::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->translate(xCo, yCo);
    painter->scale(scl, scl);
	painter->rotate(ang);

	painter->setPen(QPen(Qt::black, 3));
	painter->setBrush(QBrush(Qt::red));

	painter->drawEllipse(QPointF(0,0), diameter/2, diameter/2);

	painter->setBrush(QBrush(Qt::yellow));
	painter->drawRect(0 - 5, 0 - 5, 10, 10);
}

void Circle::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}

	QGraphicsItem::update();
	QGraphicsItem::wheelEvent(event);
}

double Circle::perimeter()
{
	return PI * diameter * scl;
}

double Circle::area()
{
	return PI * diameter * diameter * pow(scl,2) / 4;
}

void Circle::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

QPointF Circle::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}


