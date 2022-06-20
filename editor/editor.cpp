#include "editor.h"

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>

CEditor::CEditor(QWidget *parent)
    : QWidget{parent}
{
    pHistoryView = new CHistoryView(this);
    pCodeEditor = new CCodeEditor(this);
    pInputLine = new CInputLine(this);

    connect(pInputLine, &CInputLine::InputEntered, this, &CEditor::InputEntered);


    //Layouting
    QHBoxLayout* hl = new QHBoxLayout(this);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);

    splitter->addWidget(pHistoryView);

    QWidget* p_right_widget = new QWidget(this);
    QVBoxLayout* vl = new QVBoxLayout();
    p_right_widget->setLayout(vl);
    vl->addWidget(pCodeEditor);
    vl->addWidget(pInputLine);

    splitter->addWidget(p_right_widget);

    hl->addWidget(splitter);

//    QHBoxLayout* hl = new QHBoxLayout();
//    QVBoxLayout* vl = new QVBoxLayout();

//    setLayout(hl);

//    hl->addWidget(pHistoryView);

//    vl->addWidget(pCodeEditor);
//    vl->addWidget(pInputLine);

//    hl->addLayout(vl);
}

void CEditor::InputEntered(QString text)
{
    pHistoryView->WriteHistory(text);
}
