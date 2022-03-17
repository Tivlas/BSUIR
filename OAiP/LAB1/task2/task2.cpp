#include "task2.h"
#include "stdafx.h"


task2::task2(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	scene = new QGraphicsScene(this);
	cnv = new Canvas();
	cnv->setFixedSize(630, 495);
	this->setFixedSize(870, 610);

	ui.graphicsView->setScene(scene);
	scene->setSceneRect(-500, -360, 850, 570);
	ui.graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	ui.graphicsView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	ui.rotationSlider->setMinimum(0);
	ui.rotationSlider->setMaximum(360);
	shape = new Circle(0, 0, 0);
}

void task2::on_rotationSlider_valueChanged()
{
	int x = ui.rotationSlider->value();
	QString str = QString("%1").arg(x, 1);
	ui.degree->setText(str);
	shape->rotate(x);
}

void task2::on_perim_clicked()
{
	if (shape != nullptr) {
		double x = shape->perimeter();
		QString str = QString("Perimeter is %1").arg(x, 1);
		ui.lbl->setText(str);
	}
}

void task2::on_area_clicked()
{
	if (shape != nullptr) {
		double x = shape->area();
		QString str = QString("Square is %1").arg(x, 1);
		ui.lbl->setText(str);
	}
}

void task2::paintEvent(QPaintEvent* e)
{
	QPainter painter(this);
	shape->paint(&painter);
}

void task2::on_circleBtn_clicked()
{
	scene->clear();
	shape = new Circle(0, 0, 100);
	repaint();
	scene->addItem(shape);
}

void task2::on_rectBtn_clicked()
{
	scene->clear();
	shape = new Rect(200, 100);
	repaint();
	scene->addItem(shape);
}

void task2::on_squareBtn_clicked()
{
	scene->clear();
	shape = new Square(100);
	repaint();
	scene->addItem(shape);
}

void task2::on_rhombusBtn_clicked()
{
	scene->clear();
	shape = new Rhombus(200, 100);
	repaint();
	scene->addItem(shape);
}

void task2::on_hexagonBtn_clicked()
{
	scene->clear();
	shape = new Hexagon(0,0,200);
	repaint();
	scene->addItem(shape);
}

void task2::on_starBtn_clicked()
{
	scene->clear();
	shape = new Star(0, 0,50, 200);
	repaint();
	scene->addItem(shape);
}

void task2::on_trapezoidBtn_clicked()
{
	scene->clear();
	shape = new Trapezoid(0, 0, 60, 100, 50);
	repaint();
	scene->addItem(shape);
}

void task2::on_triangleBtn_clicked() {
	scene->clear();
	shape = new Triangle(0, 0, 0, 100, 100, 100);
	repaint();
	scene->addItem(shape);
}

void task2::on_canvas_clicked()
{
	cnv->show();
}

void task2::on_centre_clicked()
{
	QPointF point = shape->findCentre();
	double x = point.x();
	double y = point.y();
	QString strX = QString("x = %1").arg(x, 1);
	ui.X->setText(strX);

	QString strY = QString("y = %1").arg(y, 1);
	ui.Y->setText(strY);
}

void task2::on_diam_returnPressed()
{
	double x = ui.diam->text().toDouble();
	*(shape->changeDiam()) = x;
	scene->update();
}

void task2::on_width_returnPressed()
{
	double x = ui.width->text().toDouble();
	*(shape->changeWidth()) = x;
	scene->update();
}

void task2::on_vertices_returnPressed()
{
	int x = ui.vertices->text().toInt();
	*(shape->changeNum()) = x;
	scene->update();
}

void task2::on_XC_returnPressed()
{
	double x = ui.XC->text().toDouble();
	*(shape->changeXC()) = x;
	scene->update();
}

void task2::on_YC_returnPressed()
{
	double y = ui.YC->text().toDouble();
	*(shape->changeYC()) = y;
	scene->update();
}
