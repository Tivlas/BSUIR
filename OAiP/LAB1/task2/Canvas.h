#pragma once
#include <QtWidgets/QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include "paint.h"
#include "ui_canvas.h"

namespace Ui {
	class Canvas;
}

class Canvas : public QWidget
{
	Q_OBJECT

public:
	Canvas(QWidget* parent = nullptr);


public slots:
	void on_canvasClear_clicked();

protected:
	paint* scn;
	Ui::Canvas ui;
};
