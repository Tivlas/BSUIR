#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_vectorvisualization.h"
#include "Vector.h"


class VectorVisualization : public QMainWindow
{
    Q_OBJECT

public:
    VectorVisualization(QWidget *parent = Q_NULLPTR);
    void print(Vector<int>& vector);
private slots:
    void on_clearBtn_clicked();
    void on_pop_backBtn_clicked();
	void on_push_backBtn_clicked();
    void on_emplace_backBtn_clicked();
    void on_emplaceBtn_clicked();
    void on_resizeBtn_clicked();
	void on_reserveBtn_clicked();
	void on_assignBtn_clicked();
    void on_insertBtn_clicked();
    void on_eraseBtn_clicked();
    void on_quitBtn_clicked();

private:
    Ui::VectorVisualizationClass ui;
	Vector<int> vector;
    QTextCharFormat notInBold, inBold;
};
