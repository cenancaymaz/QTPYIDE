#ifndef CINPUTLINE_H
#define CINPUTLINE_H

#include <QtWidgets>

class CInputLine : public QWidget
{
    Q_OBJECT
public:
    explicit CInputLine(QWidget *parent = nullptr);

protected:
    bool eventFilter(QObject* obj, QEvent *e);

private:

    enum Pressed{UpButton, DownButton};

    Pressed mLastPressed;

    QLineEdit* pInputEdit;

    QPushButton* pSendButton;

    QStack<QString> mHistoryStack;
    QStack<QString> mHistoryStack2;

    void UpPressed();

    void DownPressed();

public slots:

    void AddToHistoryStack(QString Text);

private slots:

    void SendInput();

signals:

    void InputSent(QString Text);
};

#endif // CINPUTLINE_H
