#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bitset_visualization.h"
#include "BitSet.h"

class BitSet_Visualization : public QMainWindow
{
    Q_OBJECT

public:
    BitSet_Visualization(QWidget *parent = Q_NULLPTR);
    void print(const BitSet<77> &bs);
public slots:
    void on_resetBtn_clicked();
    void on_setBtn_clicked();
    void on_flipBtn_clicked();
    void on_testBtn_clicked();
    void on_anyBtn_clicked();
    void on_allBtn_clicked();
	void on_countBtn_clicked();

private:
    Ui::BitSet_VisualizationClass ui;
    BitSet<77> bs;
};
