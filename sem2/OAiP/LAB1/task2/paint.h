#include <QtWidgets/QWidget>
#include <QPainter>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <ui_canvas.h>



class paint : public QGraphicsScene
{
	Q_OBJECT

public:
	paint(QObject* parent = nullptr);
public slots:
	void mousePressEvent(QGraphicsSceneMouseEvent*);
	void mouseMoveEvent(QGraphicsSceneMouseEvent*);

protected:
	QPointF prevP;
};