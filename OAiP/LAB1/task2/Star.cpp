#include "Star.h"

Star::Star(double X, double Y,double radius, double diameter)
{
	setFlag(ItemIsMovable);
	this->radius = radius;
	this->diameter = diameter;
	xCo = X;
	yCo = Y;
}

void Star::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->translate(xCo, yCo);
	painter->scale(scl, scl);
	painter->rotate(ang);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	QPointF c(0,0);
	QPainterPath path(c);
	path.moveTo(0,0 - diameter / 2);
	int number = this->num;
	for (int i = 0; i <= number; i++) {
		path.lineTo(0 + sin((i * 2 - 1) * 3.14 / number) * radius, 0 - cos((i * 2 - 1) * 3.14 / number) * radius);
		path.lineTo(0 + sin(i * 2 * 3.14 / number) * diameter / 2 + 1, 0 - cos(i * 2 * 3.14 / number) * diameter / 2 + 1);
	}
	painter->fillPath(path, QBrush(Qt::green));
}

void Star::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {	
		scl -= 0.1;
	}
	QGraphicsItem::update();
}

double Star::perimeter()
{
	double P = 2 * num * sqrt(pow(diameter * scl / 2, 2) + pow(radius * scl, 2) - diameter * scl * cos(3.14 / num));
	return P;
}

double Star::area()
{
	double S = num * radius * scl * diameter * scl / 2 * sin(3.14 / num);
	return S;
}

void Star::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

QPointF Star::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}
