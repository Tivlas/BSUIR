#include "textEditWithInsertChecking.h"

MyTextEdit::MyTextEdit(QWidget *parent) : QTextEdit(parent), m_maxLength(0)
{
}

void MyTextEdit::setMaxLength(int maxLength)
{
    m_maxLength = maxLength;
}

void MyTextEdit::insertFromMimeData(const QMimeData *source)
{
    if (source->hasText())
    {
        QString text = source->text();
        if (text.length() + toPlainText().length() <= m_maxLength)
        {
            QTextEdit::insertFromMimeData(source);
        }
    }
    else
    {
        QTextEdit::insertFromMimeData(source);
    }
}
