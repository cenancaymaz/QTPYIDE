#include "code_editor.h"
#include <QBoxLayout>


CCodeEditor::CCodeEditor(QWidget *parent)
    : QGroupBox{parent}
{
    setTitle(tr("Code Editor"));

    pTextEdit = new QTextEdit(this);
    pRunButton = new QPushButton(tr("Run"), this);
    pRunSelectedButton = new QPushButton(tr("Run Selected"), this);

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();

    setLayout(vl);

    vl->addWidget(pTextEdit);

    hl->addWidget(pRunButton);
    hl->addWidget(pRunSelectedButton);

    vl->addLayout(hl);
}

