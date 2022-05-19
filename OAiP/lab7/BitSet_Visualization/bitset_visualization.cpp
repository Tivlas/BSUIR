#include "bitset_visualization.h"
#include "stdafx.h"


BitSet_Visualization::BitSet_Visualization(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	bs.reset();
}

void BitSet_Visualization::print(const BitSet<77>& bs)
{
	QString str = QString(bs.to_string().c_str());
	ui.lineEdit->setText(str);
}

void BitSet_Visualization::on_setBtn_clicked()
{
	QString str = ui.setEdit->text();
	if (str.isEmpty()) {
		bs.set();
	}
	else {
		if (str.toInt() >= 77) {
			QMessageBox::warning(this, "Warning", "The number is too large!");
			return;
		}
		bs.set(str.toInt());
	}
	print(bs);
}

void BitSet_Visualization::on_flipBtn_clicked()
{
	QString str = ui.flipEdit->text();
	if (str.isEmpty()) {
		bs.flip();
	}
	else {
		if (str.toInt() >= 77) {
			QMessageBox::warning(this, "Warning", "The number is too large!");
			return;
		}
		bs.flip(str.toInt());
	}
	print(bs);
}

void BitSet_Visualization::on_testBtn_clicked()
{
	bool tested;
	QString str = ui.testEdit->text();
	if (str.isEmpty()) {
		QMessageBox::warning(this, "Warning", "Enter the number");
	}
	else {
		if (str.toInt() >= 77) {
			QMessageBox::warning(this, "Warning", "The number is too large!");
			return;
		}
		tested = bs.test(str.toInt());
		ui.testEdit->setText(QString::number(tested));
		print(bs);
	}

}

void BitSet_Visualization::on_anyBtn_clicked()
{
	bool any;
	any = bs.any();
	ui.anyEdit->setText(QString::number(any));
	print(bs);

}

void BitSet_Visualization::on_allBtn_clicked()
{
	bool all;
	all = bs.all();
	ui.allEdit->setText(QString::number(all));
	print(bs);

}

void BitSet_Visualization::on_countBtn_clicked()
{
	size_t count;
	count = bs.count();
	ui.countEdit->setText(QString::number(count));
	print(bs);

}

void BitSet_Visualization::on_resetBtn_clicked()
{
	bs.reset();
	print(bs);

}
