#ifndef CINPUTLINE_H
#define CINPUTLINE_H

#include <QFrame>
#include <QLineEdit>
#include <QPushButton>

class CInputLine : public QFrame
{
    Q_OBJECT
public:
    explicit CInputLine(QWidget *parent = nullptr);

private:
    QLineEdit* pInputEdit;
    QPushButton* pSendButton;

private slots:

    void SendInput();

signals:
    void InputEntered(QString text);

};

#endif // CINPUTLINE_H
