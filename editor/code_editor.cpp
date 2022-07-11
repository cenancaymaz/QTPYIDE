#include "code_editor.h"
#include <QBoxLayout>


CCodeEditor::CCodeEditor(QWidget *parent)
    : QGroupBox{parent}
{
    setTitle(tr("Code Editor"));

    pTextEdit = new QTextEdit(this);

    pRunButton = new QPushButton(tr("Run"), this);
    connect(pRunButton, &QPushButton::clicked, this, &CCodeEditor::SendInput);

    pRunSelectedButton = new QPushButton(tr("Run Selected"), this);
    connect(pRunSelectedButton, &QPushButton::clicked, this, &CCodeEditor::SendSelected);

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();

    setLayout(vl);

    vl->addWidget(pTextEdit);

    hl->addWidget(pRunButton);
    hl->addWidget(pRunSelectedButton);

    vl->addLayout(hl);
}

void CCodeEditor::SendInput()
{
    emit InputEntered(pTextEdit->toPlainText());
}

void CCodeEditor::SendSelected()
{
    QString s = pTextEdit->textCursor().selectedText();

    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

