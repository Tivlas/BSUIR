#include "Triangle.h"


Triangle::Triangle(double x1, double y1, double x2, double y2, double x3, double y3) : x1(x1),y1(y1), x2(x2), y2(y2), x3(x3), y3(y3)
{
	setFlag(ItemIsMovable);
}

void Triangle::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
	painter->translate(xCo, yCo);
	painter->scale(scl, scl);
	painter->rotate(ang);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));	
	painter->setBrush(QBrush(Qt::red));

	QPolygon pol;
	pol << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3);
	painter->drawPolygon(pol);

	/*painter->drawLine(QPoint(x1, y1), QPoint(x2, y2));
	painter->drawLine(QPoint(x2, y2), QPoint(x3, y3));
	painter->drawLine(QPoint(x1, y1), QPoint(x3, y3));*/
}

void Triangle::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}
	QGraphicsItem::update();
}

double Triangle::area()
{
	double a = sqrt(pow(x2*scl-x1*scl,2)+ pow(y2 * scl - y1 * scl, 2));
	double b = sqrt(pow(x3 * scl - x2 * scl, 2) + pow(y3 * scl - y2 * scl, 2));
	double c = sqrt(pow(x3 * scl - x1 * scl, 2) + pow(y3 * scl - y1 * scl, 2));
	double p = (a + b + c) / 2;
	double S = sqrt(p * (p - a) * (p - b) * (p - c));
	return S;
}

void Triangle::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

double Triangle::perimeter()
{
	double a = sqrt(pow(x2 * scl - x1 * scl, 2) + pow(y2 * scl - y1 * scl, 2));
	double b = sqrt(pow(x3 * scl - x2 * scl, 2) + pow(y3 * scl - y2 * scl, 2));
	double c = sqrt(pow(x3 * scl - x1 * scl, 2) + pow(y3 * scl - y1 * scl, 2));
	return a+b+c;
}

QPointF Triangle::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}



