#include "Square.h"

Square::Square()
{
}

Square::Square(double width)
{
	setFlag(ItemIsMovable);
	this->width = width;
}

void Square::paint(QPainter* painter, const QStyleOptionGraphicsItem* opt, QWidget* widg)
{
	painter->translate(xCo, yCo);
	painter->scale(scl, scl);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	painter->setBrush(QBrush(Qt::red));
	painter->rotate(ang);

	painter->drawRect(-width / 2, -width / 2, width, width);
	
}

void Square::wheelEvent(QGraphicsSceneWheelEvent* event)
{
	if (event->delta() > 0) {
		scl += 0.1;
	}
	else {
		scl -= 0.1;
	}

	QGraphicsItem::update();
}

void Square::rotate(int val)
{
	ang = val;
	QGraphicsItem::update();
}

double Square::perimeter()
{
	return 2 * (width * scl + width * scl);;
}

double Square::area()
{
	return width * scl * width * scl;
}

QPointF Square::findCentre()
{
	QPoint res(xCo, yCo);
	return res;
}

