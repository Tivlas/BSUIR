#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bidecimal.h"
#include "Vector.h"


class Bidecimal : public QMainWindow
{
    Q_OBJECT

public:
    Bidecimal(QWidget *parent = Q_NULLPTR);
    bool k_bit(std::string str, int k);
    std::string powstr(const std::string& str, int k);
public slots:
    void on_calcBtn_clicked();

private:
    Ui::BidecimalClass ui;
    Vector<std::string> A = std::move(Vector<std::string>(10000));
    Vector<std::string> B = std::move(Vector<std::string>(10000));
    bool clicked = false;
};
