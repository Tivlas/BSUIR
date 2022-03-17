#include "Rect.h"

Rect::Rect(double width, double height)
{
	setFlag(ItemIsMovable);
	this->width = width;
	this->height = height;
}

void Rect::paint(QPainter* painter, const QStyleOptionGraphicsItem* opt, QWidget* widg)
{
	painter->translate(xCo, yCo);
	painter->scale(scl, scl);
	painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::RoundCap));
	painter->setBrush(QBrush(Qt::red));
	painter->rotate(ang);

	painter->drawRect(-width / 2, -height / 2, width, height);
}

double Rect::perimeter()
{
	return 2 * (width * scl + height * scl);
}

double Rect::area()
{
	return width * scl * height * scl;
}
