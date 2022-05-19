#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bitset_visualization.h"
#include "BitSet.h"

class BitSet_Visualization : public QMainWindow
{
    Q_OBJECT

public:
    BitSet_Visualization(QWidget *parent = Q_NULLPTR);

private:
    Ui::BitSet_VisualizationClass ui;
    BitSet<77> bs;
};
