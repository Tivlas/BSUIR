#include "Canvas.h"

Canvas::Canvas(QWidget* parent) : QWidget(parent)
{
	ui.setupUi(this);
	scn = new paint();
	ui.graphicsView->setScene(scn);
	scn->setSceneRect(0, 0, 500, 300);
	//ui.graphicsView->setFixedSize(500,300);

}

void Canvas::on_canvasClear_clicked() {
	scn->clear();
}




