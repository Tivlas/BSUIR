#include "Trapezoid.h"

Trapezoid::Trapezoid(double x1, double y1, double x2, double y2, double width) :x1(x1), y1(y1), x2(x2), y2(y2)
{
	setFlag(ItemIsMovable);
	this->width = width;
}

void Trapezoid::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{

	if (x2 >= x1 && y2 >= y1)
	{
		painter->translate(xCo, yCo);
		painter->scale(scl, scl);
		painter->rotate(ang);
		painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
		painter->setBrush(QBrush(Qt::red));
		x3 = x2 + width;
		y3 = y2;
		x4 = x1 + 2 * (x2 - x1) + width;
		y4 = y1;
		double h = y2 - y1;
		double a = sqrt(pow(x2 * scl - x3 * scl, 2) + pow(y2 * scl - y3 * scl, 2));
		double b = sqrt(pow(x1 * scl - x4 * scl, 2) + pow(y1 * scl - y4 * scl, 2));
		QPolygon pol;

		pol << QPoint( - b / 2,  - h / 2) << QPoint( - a / 2,  + h / 2) <<
			QPoint( + a / 2,  + h / 2) << QPoint( + b / 2,  - h / 2);

		painter->drawPolygon(pol);
	}
}

void Trapezoid::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}
	QGraphicsItem::update();
}

double Trapezoid::perimeter()
{
	double a = sqrt(pow(x2 * scl - x3 * scl, 2) + pow(y2 * scl - y3 * scl, 2));
	double b = sqrt(pow(x1 * scl - x4 * scl, 2) + pow(y1 * scl - y4 * scl, 2));
	double c = sqrt(pow(x1 * scl - x2 * scl, 2) + pow(y1 * scl - y2 * scl, 2));
	double d = sqrt(pow(x3 * scl - x4 * scl, 2) + pow(y3 * scl - y4 * scl, 2));
	return a + b + c + d;
}

double Trapezoid::area()
{
	double a = sqrt(pow(x2 * scl - x3 * scl, 2) + pow(y2 * scl - y3 * scl, 2));
	double b = sqrt(pow(x1 * scl - x4 * scl, 2) + pow(y1 * scl - y4 * scl, 2));
	double h = fabs(y2) - fabs(y1);
	double S = (a + b) / 2 * h;
	return S;
}

void Trapezoid::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

QPointF Trapezoid::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}
