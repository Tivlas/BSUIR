#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_u_map_visualization.h"
#include "Unordered_map.h"

using Umap = Unordered_map<int, QString>;
using iQsPair = std::pair<int, QString>;

class U_map_Visualization : public QMainWindow
{
	Q_OBJECT

public:
	U_map_Visualization(QWidget* parent = Q_NULLPTR);
	void print(const Umap& umap);
private slots:
	void on_insertBtn_clicked();
	void on_eraseBtn_clicked();
	void on_itEraseBtn_clicked();
	void on_operatorBtn_clicked();
	void on_containsBtn_clicked();
	void on_bucket_sizeBtn_clicked();
	void on_bucketBtn_clicked();
private:
	Ui::U_map_VisualizationClass ui;
	Umap u_map;
};
