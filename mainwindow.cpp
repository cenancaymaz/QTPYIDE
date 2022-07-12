#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setDockNestingEnabled(true);


    QDockWidget *dock = new QDockWidget(tr("Console"),this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pOutputView = new CConsoleView(dock);
    dock->setWidget(pOutputView);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, dock);

    dock = new QDockWidget(tr("Code Editor"),this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pCodeEditor = new CCodeEditor(dock);
    dock->setWidget(pCodeEditor);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock->setTitleBarWidget(0);
    addDockWidget(Qt::RightDockWidgetArea, dock);


    dock = new QDockWidget(this);
    dock->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pInputLine = new CInputLine(dock);
    pInputLine->setFixedHeight(80);
    dock->setWidget(pInputLine);
    dock->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, dock);


    QWidget* p_widget = new QWidget(this);
    p_widget->setFixedWidth(0);
    setCentralWidget(p_widget);


    dock = new QDockWidget(this);
    dock->setAllowedAreas(Qt::BottomDockWidgetArea);
    QLabel* deneme = new QLabel("deneme", dock);
    dock->setWidget(deneme);
    dock->setFeatures(QDockWidget::NoDockWidgetFeatures);

    addDockWidget(Qt::BottomDockWidgetArea, dock);


    connect(pInputLine, &CInputLine::InputEntered, pOutputView, &CConsoleView::WriteInput);
    connect(pCodeEditor, &CCodeEditor::InputEntered, pOutputView, &CConsoleView::WriteInput);


    //CEditor* p_editor = new CEditor(this);
    //setCentralWidget(p_editor);

    resize(1400, 800);
}

MainWindow::~MainWindow()
{
}

