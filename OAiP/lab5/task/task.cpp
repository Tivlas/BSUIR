#include "task.h"
#include "stdafx.h"

task::task(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    notInBold = ui.firstLoop->currentCharFormat();
    inBold.setFontWeight(QFont::Bold);
}

void task::on_printBtn_clicked()
{
    if (list.size() != 0)
    {
        ui.firstLoop->setText("");
        ui.secondLoop->setText("");
        ui.listText->setText("");
        list.createLoops();
        ui.firstLoop->setCurrentCharFormat(inBold);
        ui.secondLoop->setCurrentCharFormat(inBold);
        ui.listText->setCurrentCharFormat(inBold);
		
        ui.listText->append("Список:");
        ui.firstLoop->append("Между максимальным и минимальным:");
        ui.secondLoop->append("Остальные элементы:");
		
        ui.firstLoop->setCurrentCharFormat(notInBold);
        ui.secondLoop->setCurrentCharFormat(notInBold);
        ui.listText->setCurrentCharFormat(notInBold);
		
        ui.listText->append(list.printList());
        ui.firstLoop->append(list.printFirstLoop());
        ui.secondLoop->append(list.printSecondLoop());
    }
}

void task::on_clearBtn_clicked()
{
    list.clear();
	ui.firstLoop->setText("");
	ui.secondLoop->setText("");
	ui.listText->setText("");
}

void task::on_pushBtn_clicked()
{
    list.add(ui.pushEdit->text().toInt());
    ui.pushEdit->clear();
}
