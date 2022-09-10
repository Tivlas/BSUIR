#include "Rhombus.h"

Rhombus::Rhombus(double diag1, double diag2) : diag1(diag1),diag2(diag2)
{
	setFlag(ItemIsMovable);
	side = sqrt(pow(diag1 / 2, 2) + pow(diag2 / 2, 2));
}

void Rhombus::paint(QPainter* painter, const QStyleOptionGraphicsItem*, QWidget*)
{
	painter->translate(xCo, yCo);
	painter->scale(scl, scl);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	painter->setBrush(QBrush(Qt::red));
	painter->rotate(ang);

	x1 = 0; x3 = 0;
	x2 = -(diag2 / 2); x4 = (diag2 / 2); // first diag is vertical

	y1 = -(diag1 / 2); y3 = (diag1 / 2);
	y2 = 0; y4 = 0;

	QPolygon pol;
	pol << QPoint(x1, y1) << QPoint(x2, y2) << QPoint(x3, y3) << QPoint(x4, y4);
	painter->drawPolygon(pol);
}

void Rhombus::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}

	QGraphicsItem::update();
}

double Rhombus::perimeter()
{
	return side * scl * 4;
}

double Rhombus::area()
{
	return diag1 * scl * diag2 * scl / 2;
}

void Rhombus::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

QPointF Rhombus::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}
