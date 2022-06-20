#ifndef CTEXTEDITOR_H
#define CTEXTEDITOR_H

#include <QTextEdit>

class CTextEditor : public QTextEdit
{
    Q_OBJECT
public:
    explicit CTextEditor(QWidget *parent = nullptr);

signals:

};

#endif // CTEXTEDITOR_H
