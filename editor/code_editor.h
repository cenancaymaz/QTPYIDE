#ifndef CTEXTEDITOR_H
#define CTEXTEDITOR_H

#include <QGroupBox>
#include <QTextEdit>
#include <QPushButton>

class CCodeEditor : public QGroupBox
{
    Q_OBJECT
public:
    explicit CCodeEditor(QWidget *parent = nullptr);

private:
    QTextEdit* pTextEdit;
    QPushButton* pRunButton;
    QPushButton* pRunSelectedButton;

signals:

};

#endif // CTEXTEDITOR_H
