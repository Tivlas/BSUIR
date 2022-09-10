#include "task1.h"
#include "stdafx.h"


LAB1::LAB1(QWidget* parent)
	: QWidget(parent)
{
	ui.setupUi(this);
	ui.Start->move(890, 570);
	ui.turnOn->move(730, 570);
}

void LAB1::on_turnOn_clicked()
{
	ui.turnOn->setEnabled(false);
	connect(timer, SIGNAL(timeout()), this, SLOT(turnOnLights()));
	timer->start(1000 / 2);
}

void LAB1::moving()
{
	bus->openL();
	bus->openR();
	repaint();
}

void LAB1::turnOnLights()
{
	repaint();
}

void LAB1::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	bus->draw(&painter);
	bus->drawLights(&painter);
}

void LAB1::on_Start_clicked() {
	ui.Start->setEnabled(false);
	for (int i = 0; i < 30; i++) {
		QTimer::singleShot(1000/60, this, SLOT(moving()));
	}
}

