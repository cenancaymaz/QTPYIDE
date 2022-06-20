#ifndef CEDITOR_H
#define CEDITOR_H

#include <QWidget>

#include <QTextEdit>

class CEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CEditor(QWidget *parent = nullptr);

private:

    QTextEdit *pTextEditor;


signals:

};

#endif // CEDITOR_H
