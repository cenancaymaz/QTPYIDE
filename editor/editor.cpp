#include "editor.h"

#include <QLabel>
#include <QVBoxLayout>

CEditor::CEditor(QWidget *parent)
    : QWidget{parent}
{
    pTextEditor = new QTextEdit(this);

    QVBoxLayout* vl = new QVBoxLayout(this);

    vl->addWidget(pTextEditor);
}
