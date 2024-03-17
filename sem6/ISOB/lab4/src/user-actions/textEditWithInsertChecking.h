#ifndef TEXTEDITWITHINSERTCHECKING_H
#define TEXTEDITWITHINSERTCHECKING_H

#include <QTextEdit>
#include <QWidget>
#include <QMimeData>

class MyTextEdit : public QTextEdit
{
    Q_OBJECT

public:
    explicit MyTextEdit(QWidget *parent = nullptr);

    void setMaxLength(int maxLength);

protected:
    void insertFromMimeData(const QMimeData *source) override;

private:
    int m_maxLength;
};


#endif // TEXTEDITWITHINSERTCHECKING_H
