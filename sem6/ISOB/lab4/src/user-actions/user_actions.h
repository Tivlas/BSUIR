#ifndef MAIN_H
#define MAIN_H

#include <QWidget>
#include <QListWidgetItem>

namespace Ui {
class user_actions;
}

class user_actions : public QWidget
{
    Q_OBJECT

public:
    explicit user_actions(size_t level, QWidget *parent = nullptr);
    void set_privilege_lvl(int lvl = 2);
    ~user_actions();

signals:
    void sign_out();

private slots:


    void on_searchFilesBtn_clicked();

    void on_foundFiles_itemClicked(QListWidgetItem *item);

    void on_saveBtn_clicked();

    void on_signOutBtn_clicked();

private:
    size_t privilege_lvl;
    Ui::user_actions *ui;
};

#endif // MAIN_H
