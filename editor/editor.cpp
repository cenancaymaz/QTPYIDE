#include "editor.h"

#include <QtWidgets>

#include <QLabel>
#include <QBoxLayout>
#include <QSplitter>
#include <QDockWidget>


#include <QSizePolicy>

CEditor::CEditor(QWidget *parent)
    : QFrame{parent}
{
    pOutputView = new CConsoleView(this);
    pCodeEditor = new CCodeEditor(this);
    pInputLine = new CInputLine(this);

    connect(pInputLine, &CInputLine::InputEntered, pOutputView, &CConsoleView::WriteInput);
    connect(pCodeEditor, &CCodeEditor::InputEntered, pOutputView, &CConsoleView::WriteInput);


    //Layouting
    QVBoxLayout* vl = new QVBoxLayout(this);

    QSplitter* splitter = new QSplitter(Qt::Horizontal, this);


    QDockWidget* p_dock_widget = new QDockWidget("deneme", this);
    //p_dock_widget->setWindowFlags(Qt::FramelessWindowHint);
    p_dock_widget->setFeatures(QDockWidget::AllDockWidgetFeatures);
    p_dock_widget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    p_dock_widget->setWidget(pOutputView);

    splitter->addWidget(p_dock_widget);


    QDockWidget* p_dock_widget_2 = new QDockWidget("deneme2", this);
    //p_dock_widget->setWindowFlags(Qt::FramelessWindowHint);
    p_dock_widget_2->setFeatures(QDockWidget::AllDockWidgetFeatures);
    p_dock_widget_2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    p_dock_widget_2->setWidget(pCodeEditor);

    splitter->addWidget(p_dock_widget_2);



    //splitter->addWidget(pCodeEditor);
    splitter->setSizePolicy(QSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding));

    vl->addWidget(splitter);
    vl->addWidget(pInputLine);
}

void CEditor::InputEntered(QString text)
{
    pOutputView->WriteInput(text);
}
