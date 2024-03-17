#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "db/db.h"
#include <QDebug>
#include "DB_connection_data.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    qDebug() << DB::connect("QPSQL", DB_HOST_NAME, DB_NAME, DB_USER_NAME, DB_PASSWORD);
    ui->setupUi(this);
    
    lg = new login();
    ua = new user_actions(-1);

    set_lg();

    connect(lg, SIGNAL(sign_in(int)), this, SLOT(set_main(int)));
    connect(ua, SIGNAL(sign_out()), this, SLOT(set_lg()));
}

MainWindow::~MainWindow()
{
    delete lg;
    delete ua;
    delete ui;
}

void MainWindow::set_lg() {
    main_size = ua->size();
    ua->hide();
    centralWidget()->setParent(0);

    this->setCentralWidget(lg);
    lg->show();
}

void MainWindow::set_main(int level) {
    login_size = lg->size();
    lg->hide();
    centralWidget()->setParent(0);
    this->setCentralWidget(ua);
    ua->set_privilege_lvl(level);
    ua->show();
}
