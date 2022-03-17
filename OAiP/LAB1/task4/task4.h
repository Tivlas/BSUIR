#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_task4.h"
#include "Student.h"

class task4 : public QMainWindow
{
    Q_OBJECT

public:
    int counter = 0;
    task4(QWidget *parent = Q_NULLPTR);
    void groupSort(Student*&, int , int);
    void fullNameSort(Student*&, int, int);
    void arrToFile(Student*,int);

    int findStudent(Student*, int, QString);
    Student* findDebtors(Student*&);
    Student* findDrop(Student*&);

public slots:
    void on_readBtn_clicked();
    void on_appendBtn_clicked();
    void on_overwriteBtn_clicked();
    void on_addstudentBtn_clicked();

    void on_nameEdit_returnPressed();
    void on_groupNumberEdit_returnPressed();
    void on_winterSessionEdit_returnPressed();
    void on_summerSessionEdit_returnPressed();
    void on_searchByNameEdit_returnPressed();
    void on_saveBtn_clicked();
    void on_saveAllBtn_clicked();
    void on_sortBtn_clicked();
    void on_debtorsBtn_clicked();
    void on_saveDebtorsBtn_clicked();
    void on_dropOutBtn_clicked();
    void on_saveDropOutBtn_clicked();
    void on_getBtn_clicked();



    void on_quitBtn_clicked() { QApplication::quit(); }
private:
    Ui::task4Class ui;
    Student* students;
    Student* debtors;
    Student* dropOut;

    QString fileName;
    int studentsSize = 0;
    int debtorsSize = 0;
    int dropoutSize = 0;
};
