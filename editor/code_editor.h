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

private slots:

    void SendInput();
    void SendSelected();

signals:
    void InputEntered(QString text);

};

#endif // CTEXTEDITOR_H
