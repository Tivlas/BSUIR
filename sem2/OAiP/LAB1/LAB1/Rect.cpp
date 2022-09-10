
#include "Rect.h"

Rect::Rect(int x, int y, int w, int l):m_x(x), m_y(y), m_wid(w), m_len(l)
{
}

void Rect::draw(QPainter* painter)
{
	painter->setPen(QPen(Qt::black, 5));
	painter->setBrush(QBrush(Qt::darkBlue));
	painter->drawRect(m_x, m_y, m_wid, m_len);
}

void Rect::openDoorLeft()
{
	m_x--;
}

void Rect::openDoorRight()
{
	m_x++;
}

Bus::Bus(int m_x,int m_y):Rect(m_x,m_y), door1(m_x+60,m_y,60,180), door2(m_x+120, m_y, 60, 180), 
                                         door3(m_x+460, m_y, 60, 180),door4(m_x+520,m_y,60,180)
{
}

void Bus::draw(QPainter* painter)
{
    painter->setPen(QPen(Qt::black, 5));
	painter->setBrush(QBrush(Qt::green));

	//bus
	QPolygon pol;
	pol << QPoint(m_x, m_y) << QPoint(m_x+800, m_y) << QPoint(m_x+800, m_y+180) << QPoint(m_x+680, m_y+180) << 
		   QPoint(m_x+650, m_y+150) << QPoint(m_x+610, m_y+150) <<
		   QPoint(m_x+580, m_y+180) << QPoint(m_x+280, m_y+180) << QPoint(m_x+250, m_y+150) <<
		   QPoint(m_x+210, m_y+150) << QPoint(m_x+180, m_y+180) << QPoint(m_x, m_y+180);
	painter->drawPolygon(pol);

	//wheels
	painter->setPen(QPen(Qt::black, 7));
	painter->setBrush(QBrush(Qt::gray));
	painter->drawEllipse(m_x + 195, m_y + 155, 70, 70);
	painter->drawEllipse(m_x + 595, m_y + 155, 70, 70);

	//interior

	/*painter->eraseRect(m_x + 90, m_y, 60, 180);
	painter->eraseRect(m_x + 490, m_y, 60, 180);*/
	painter->setPen(QPen(Qt::black, 5));
	painter->drawRect(m_x+90, m_y, 60, 180);
	painter->drawRect(m_x+490, m_y, 60, 180);

	//windows
	painter->setBrush(QBrush(Qt::cyan));
	painter->drawRect(m_x + 220, m_y+20, 200, 80);
	painter->drawRect(m_x + 620, m_y+20, 180, 80);

	//doors
	door1.draw(painter);
	door2.draw(painter);
	door3.draw(painter);
	door4.draw(painter);
}

void Bus::drawLights(QPainter* painter)
{
	//lights
	static int n = -30;
	painter->setPen(QPen(Qt::black, 5));
	if (n % 2 == 0) {
		painter->setBrush(QBrush(Qt::red));
	}
	else {
		painter->setBrush(QBrush(Qt::yellow));
	}
	painter->drawRect(m_x + 775, m_y + 130, 25, 15);
	painter->drawRect(m_x, m_y + 130, 25, 15);
	n++;
}

void Bus::openL()
{
	door1.openDoorLeft();
	door3.openDoorLeft();
}

void Bus::openR()
{
	door2.openDoorRight();
	door4.openDoorRight();
}
