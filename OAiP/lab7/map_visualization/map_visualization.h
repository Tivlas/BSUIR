#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_map_visualization.h"
#include "map.h"

class map_visualization : public QMainWindow
{
    Q_OBJECT

public:
    map_visualization(QWidget *parent = Q_NULLPTR);
	void print(const map<int, QString>& mp);
public slots:
	void on_searchBtn_clicked();
	void on_removeBtn_clicked();
	void on_addBtn_clicked();
	void on_clearBtn_clicked();
	void on_operatorBtn_clicked();
private:
    Ui::map_visualizationClass ui;
    map<int,QString> mp;
};
