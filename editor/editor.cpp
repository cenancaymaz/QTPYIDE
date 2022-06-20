#include "editor.h"

#include <QLabel>
#include <QBoxLayout>
#include <QDebug>

CEditor::CEditor(QWidget *parent)
    : QWidget{parent}
{
    pHistoryView = new CHistoryView(this);
    pTextEditor = new CTextEditor(this);
    pInputLine = new CInputLine(this);

    connect(pInputLine, &CInputLine::InputEntered, this, &CEditor::InputEntered);

    QHBoxLayout* hl = new QHBoxLayout();
    QVBoxLayout* vl = new QVBoxLayout();

    setLayout(hl);

    hl->addWidget(pHistoryView);

    vl->addWidget(pTextEditor);
    vl->addWidget(pInputLine);

    hl->addLayout(vl);
}

void CEditor::InputEntered(QString text)
{
    pHistoryView->WriteHistory(text);
}
