#ifndef CBORDERLESSBUTTON_H
#define CBORDERLESSBUTTON_H

#include <QtWidgets>

class CBorderlessButton : public QPushButton
{
    Q_OBJECT
public:
    explicit CBorderlessButton(QString text, QWidget *parent = nullptr);

    explicit CBorderlessButton(QIcon icon, QWidget *parent = nullptr);

private:

    void SetStyle();

signals:

};

#endif // CBORDERLESSBUTTON_H
