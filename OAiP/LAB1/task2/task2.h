#pragma once

#include <QtWidgets/QWidget>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QTimer>
#include "ui_task2.h"
#include "Canvas.h"
#include "shape.h"
#include "triangle.h"
#include "circle.h"
#include "Rect.h"
#include "Square.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include "Star.h"
#include "Trapezoid.h"


class task2 : public QWidget
{
	Q_OBJECT

public:
	task2(QWidget* parent = Q_NULLPTR);

public slots:
	void on_triangleBtn_clicked();
	void on_circleBtn_clicked();
	void on_squareBtn_clicked();
	void on_rectBtn_clicked();
	void on_rhombusBtn_clicked();
	void on_hexagonBtn_clicked();
	void on_starBtn_clicked();
	void on_trapezoidBtn_clicked();


	
	void on_rotationSlider_valueChanged();
	void on_perim_clicked();
	void on_area_clicked();
	void on_canvas_clicked();
	void on_centre_clicked();
	void on_diam_returnPressed();
	void on_width_returnPressed();
	void on_vertices_returnPressed();
	void on_XC_returnPressed();
	void on_YC_returnPressed();


	void paintEvent(QPaintEvent*);
	
private:
	Ui::task2Class ui;
	Shape* shape = nullptr;
	QGraphicsScene* scene;
	

	Canvas* cnv;
};
