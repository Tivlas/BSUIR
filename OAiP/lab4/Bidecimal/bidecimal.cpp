#include "bidecimal.h"
#include "stdafx.h"

Bidecimal::Bidecimal(QWidget* parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->setFixedHeight(139);
	this->setFixedWidth(669);
}

bool Bidecimal::k_bit(std::string str, int k)
{
	int bit, i = 0;
	int length = str.size();

	for (; i != k; i++)
	{
		bit = (str[length - 1] - '0') % 2;
		str[length - 1] = str[length - 1] - bit;

		for (int j = length - 1; j >= 0; j--)
		{
			if ((str[j] - '0') % 2 == 0)
			{
				str[j] = (str[j] - '0') / 2 + '0';
			}
			else
			{
				str[j] = (str[j] - '0') / 2 + '0';

				if (j != length - 1)
				{
					str[j + 1] = str[j + 1] + 5;
				}

			}
		}
	}

	return bit;
}

std::string Bidecimal::powstr(const std::string& str, int k)
{
	std::string deg = "1";

	if (k == 0)
	{
		return("1");
	}

	for (int i = 0; i < k; i++)
	{
		deg += '0';
	}

	for (int i = deg.size() - 1, j = str.size() - 1; i >= 0 && j >= 0; i--, j--)
	{
		deg[i] = str[j];
	}

	return deg;
}

void Bidecimal::on_calcBtn_clicked() {
	A[0] = "0";

	int counterB = 0, counterA = 1, k = 1, counterBIN = 1, tmp;

	int n = ui.numberEdit->text().toInt();
	if (n < 1 || n>10000) {
		QMessageBox::warning(this, "Внимание", "Число должно принадлежать промежутку [1,10000].\nВведите число заново");
		ui.numberEdit->clear();
		return;
	}

	while (n >= counterBIN)
	{
		tmp = counterB = 0;

		for (int i = 0; i < counterA; i++)
		{
			if (!k_bit(A[i], k))
			{
				A[tmp] = A[i];
				tmp++;
			}
		}

		counterA = tmp;

		for (int i = 0; i < counterA; i++)
		{
			B[i] = powstr(A[i], k - 1);
			counterB++;
		}

		for (int i = 0; i < counterB; i++)
		{
			A[counterA] = B[i];
			counterA++;
		}

		counterBIN += counterB;
		k++;
	}
	ui.numberEdit->clear();
	ui.plainTextEdit->setPlainText(QString(B[counterB - (counterBIN - n)].c_str()));
}