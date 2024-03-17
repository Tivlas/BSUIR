#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "login/login.h"
#include "user-actions/user_actions.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();




private slots:
    void set_main(int role_id);
    void set_lg();

private:
    Ui::MainWindow *ui;
    login *lg;
    user_actions *ua;
    QSize login_size, main_size;
};
#endif // MAINWINDOW_H
