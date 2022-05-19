#include "bitset_visualization.h"
#include "stdafx.h"


BitSet_Visualization::BitSet_Visualization(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    bs.reset();
}
