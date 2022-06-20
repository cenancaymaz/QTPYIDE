#ifndef CINPUTLINE_H
#define CINPUTLINE_H

#include <QWidget>
#include <QLineEdit>

class CInputLine : public QWidget
{
    Q_OBJECT
public:
    explicit CInputLine(QWidget *parent = nullptr);

private:
    QLineEdit* pInputEdit;

private slots:

    void SendInput();

signals:
    void InputEntered(QString text);

};

#endif // CINPUTLINE_H
