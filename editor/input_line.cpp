#include "input_line.h"

#include <QHBoxLayout>

CInputLine::CInputLine(QWidget *parent)
    : QWidget{parent}
{
    pInputEdit = new QLineEdit(this);
    connect(pInputEdit, &QLineEdit::returnPressed, this, &CInputLine::SendInput);

    QHBoxLayout* hl = new QHBoxLayout(this);

    hl->addWidget(pInputEdit);
}

void CInputLine::SendInput()
{
    emit InputEntered(pInputEdit->text());
    pInputEdit->clear();
}
