#include "login.h"
#include "ui_login.h"
#include "../db/db.h"
#include <QMessageBox>
#include <QFile>
#include <QFileInfo>

login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    ui->setupUi(this);

    this->ui->login_in->setMaxLength(30);
    this->ui->password_in->setMaxLength(30);
}

login::~login()
{
    delete ui;
}

void login::on_pushButton_clicked()
{
    QString login = this->ui->login_in->text(),
        password = this->ui->password_in->text();

    if (login.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Warning", "Empty fields");
        return;
    }

    auto lvl = DB::authenticate(login, password).value_or(-1);

    if (lvl != -1) {
        qDebug() << "lvl: " << lvl;
        emit sign_in(lvl);
    } else {
        QMessageBox::critical(this, "Error", "Incorrect login or password");
    }
}

