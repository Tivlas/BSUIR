#include "paint.h"

paint::paint(QObject* parent) : QGraphicsScene(parent) {

}
void paint::mousePressEvent(QGraphicsSceneMouseEvent* ev)
{
	QColor col = QColor(rand() % 255, rand() % 255, rand() % 255);
	addEllipse(ev->scenePos().x(),
		ev->scenePos().y(),
		7, 7, QPen(Qt::NoPen),col);
	prevP = ev->scenePos();
}

void paint::mouseMoveEvent(QGraphicsSceneMouseEvent* ev)
{
	QColor col = QColor(rand() % 255, rand() % 255, rand() % 255);
	addLine(prevP.x(), prevP.y(), ev->scenePos().x(), ev->scenePos().y(), QPen(col, 7, Qt::SolidLine, Qt::RoundCap));
	prevP = ev->scenePos();
}
