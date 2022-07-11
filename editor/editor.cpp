#include "editor.h"

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>

#include <QSizePolicy>

CEditor::CEditor(QWidget *parent)
    : QWidget{parent}
{

    pOutputView = new CConsoleView(this);
    pCodeEditor = new CCodeEditor(this);
    pInputLine = new CInputLine(this);

    connect(pInputLine, &CInputLine::InputEntered, this, &CEditor::InputEntered);
    connect(pCodeEditor, &CCodeEditor::InputEntered, this, &CEditor::InputEntered);


    //Layouting
    QVBoxLayout* vl = new QVBoxLayout(this);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    splitter->addWidget(pOutputView);
    splitter->addWidget(pCodeEditor);
    //splitter->setSizePolicy()
    splitter->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    vl->addWidget(splitter);
    vl->addWidget(pInputLine);
}

void CEditor::InputEntered(QString text)
{
    pOutputView->WriteInput(text);
}
