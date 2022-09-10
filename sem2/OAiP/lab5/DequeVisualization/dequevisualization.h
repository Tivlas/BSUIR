#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_dequevisualization.h"
#include "Deque.h"

class DequeVisualization : public QMainWindow
{
    Q_OBJECT

public:
    DequeVisualization(QWidget *parent = Q_NULLPTR);
    void print(Deque<int>& deq);
private slots:
    void on_insertBtn_clicked();
    void on_eraseBtn_clicked();
    void on_push_backBtn_clicked();
    void on_push_frontBtn_clicked();
    void on_clearBtn_clicked();
    void on_pop_backBtn_clicked();
    void on_pop_frontBtn_clicked();
    void on_resizeBtn_clicked();
    void on_quitBtn_clicked();
private:
    Ui::DequeVisualizationClass ui;
    Deque<int> deq;
};
