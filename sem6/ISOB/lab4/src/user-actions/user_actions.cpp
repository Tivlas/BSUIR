#include "user_actions.h"
#include "ui_user_actions.h"

#include <QRegularExpression>
#include <QRegularExpressionValidator>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QDirIterator>
#include <QTimer>
#include "textEditWithInsertChecking.h"

user_actions::user_actions(size_t level, QWidget *parent) :
    QWidget(parent),
    privilege_lvl(level),
    ui(new Ui::user_actions)
{
    ui->setupUi(this);
    ui->dirPath->setMaxLength(100);

    MyTextEdit* mte = new MyTextEdit(ui->frame_4);
    ui->textEdit = mte;
    mte->setObjectName("textEdit");
    mte->setEnabled(true);
    mte->setGeometry(QRect(20, 20, 311, 401));
    mte->setMaxLength(10000);
    setGeometry(0,0, 500, 500);
}

void user_actions::set_privilege_lvl(int lvl) {
    privilege_lvl = lvl;
    if (privilege_lvl > 0) {
        ui->saveBtn->hide();
    } else {
        ui->saveBtn->show();
    }
}

user_actions::~user_actions()
{
    delete ui;
}


void user_actions::on_searchFilesBtn_clicked()
{
    QString dirPath = ui->dirPath->text();
    QFileInfo fileInfo(dirPath);
    QString canonicalDirPath = fileInfo.canonicalFilePath();

    if (canonicalDirPath.startsWith("D:/University/BSUIR/sem6/ISOB/lab4/user-txt-files")) {
        const qint64 maxSize = 1024 * 1024;
        int maxFileCnt = 100;

        QDirIterator it(canonicalDirPath, QStringList() << "*.*", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filePath = it.next();
            QFileInfo fileInfo(filePath);
            qint64 fileSize = fileInfo.size();

            if (fileSize > maxSize) {
                QMessageBox::warning(this, "Warning", filePath + " size of file exceeds the limit of 1 MB (skipped).");
                continue;
            }

            ui->foundFiles->addItem(filePath);
            maxFileCnt--;
            if(maxFileCnt == 0) {
                QMessageBox::warning(this, "Warning", "Maximum number of files is 100 (other skipped).");
            }

        }
    } else {
        QMessageBox::critical(this, "Error", "The specified directory path is invalid.");
    }

}


void user_actions::on_foundFiles_itemClicked(QListWidgetItem *item)
{
    if (privilege_lvl > 1) {
        QMessageBox::critical(this, "Error", "You are not allowed to read files.");
    } else {
        QString fileName = item->text();
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly)) {
            qDebug() << "Open file";
            QTextStream stream(&file);
            ui->textEdit->setPlainText(stream.readAll());
            file.close();
        }
    }
}


void user_actions::on_saveBtn_clicked()
{
    ui->saveBtn->setEnabled(false);

    QTimer* timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(1000);

    QObject::connect(timer, &QTimer::timeout, [timer, this](){
        ui->saveBtn->setEnabled(true);
        timer->deleteLater();
    });
    timer->start();


    if (ui->foundFiles->currentItem() == nullptr) return;
    QString fileName = ui->foundFiles->currentItem()->text();
    QFile file(fileName);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << ui->textEdit->toPlainText();
        file.close();
    }
}




void user_actions::on_signOutBtn_clicked()
{
    emit sign_out();
}

