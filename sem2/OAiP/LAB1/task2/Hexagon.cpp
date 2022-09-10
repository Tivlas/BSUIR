#include "Hexagon.h"

Hexagon::Hexagon(double xCo, double yCo, double diameter)
{
	setFlag(ItemIsMovable);
	this->diameter = diameter;
	this->radius = sqrt(3) / 4 * diameter;
	this->xCo = xCo;
	this->yCo = yCo;
}

void Hexagon::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->translate(xCo, yCo);
	this->radius = sqrt(3) / 4 * diameter;
	painter->scale(scl, scl);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	painter->setBrush(QBrush(Qt::red));
	painter->rotate(ang);

	QPolygon pol;
	double l = sqrt(pow(diameter / 2, 2) - pow(radius, 2));
	pol << QPoint(0,0+diameter/2) << QPoint(0+radius,0+l) << QPoint(0+radius,0-l) << 
		   QPoint(0,0-diameter/2) << QPoint(0 - radius, 0 - l) << QPoint(0 - radius, 0 + l);

	painter->drawPolygon(pol);
}

void Hexagon::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}

	QGraphicsItem::update();
}

double Hexagon::perimeter()
{
	return 3*diameter*scl;
}

double Hexagon::area()
{
	return 1.5 * diameter * scl * diameter * scl * sqrt(3) / 4;
}

void Hexagon::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

QPointF Hexagon::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}
