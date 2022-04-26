#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_bidecimal.h"
#include "Vector.h"


class Bidecimal : public QMainWindow
{
    Q_OBJECT

public:
    Bidecimal(QWidget *parent = Q_NULLPTR);

private:
    Ui::BidecimalClass ui;
    Vector<std::string> A = std::move(Vector<std::string>(10000));
};
