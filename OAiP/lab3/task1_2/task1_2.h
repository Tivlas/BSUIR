#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task1_2.h"
#include "List.h"

class task1_2 : public QMainWindow
{
    Q_OBJECT

public:
    task1_2(QWidget *parent = Q_NULLPTR);
	bool dateValidation(const QString& date);
public slots:
    void on_quitBtn_clicked();
    void on_addBtn_clicked();
    void on_printBtn_clicked();
    void on_deleteOneByNameBtn_clicked();
    void on_deleteAllByNameBtn_clicked();
    void on_printByPriceBtn_clicked();
    void on_printByNameBtn_clicked();
    void on_printByTimeBtn_clicked();
    void on_saveLastBtn_clicked();
    void on_saveAllBtn_clicked();
    void on_readToListBtn_clicked();
private:
    Ui::task1_2Class ui;
    QString fileName;
    List list;
    List waiting;
};
