#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setDockNestingEnabled(true);

    QDockWidget *dock1 = new QDockWidget(tr("Files"),this);
    dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pFilesView = new CFilesView(dock1);
    dock1->setWidget(pFilesView);
    dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, dock1);





    QDockWidget *dock2 = new QDockWidget(tr("Code Editor"),this);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pCodeEditor = new CCodeEditor(dock2);
    dock2->setWidget(pCodeEditor);
    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock2->setTitleBarWidget(0);
    addDockWidget(Qt::RightDockWidgetArea, dock2);

    QDockWidget *dock3 = new QDockWidget(tr("Console"),this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pOutputView = new CConsoleView(dock3);
    dock3->setWidget(pOutputView);
    dock3->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, dock3);


    QDockWidget *dock4 = new QDockWidget(this);
    dock4->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pInputLine = new CInputLine(dock4);
    pInputLine->setFixedHeight(80);
    dock4->setWidget(pInputLine);
    dock4->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::RightDockWidgetArea, dock4);




    QWidget* p_widget = new QWidget(this);
    p_widget->setFixedWidth(0);
    setCentralWidget(p_widget);


    QDockWidget *dock5 = new QDockWidget(this);
    dock5->setAllowedAreas(Qt::BottomDockWidgetArea);
    QLabel* deneme = new QLabel("deneme", dock5);
    dock5->setWidget(deneme);
    dock5->setFeatures(QDockWidget::NoDockWidgetFeatures);

    addDockWidget(Qt::BottomDockWidgetArea, dock5);


    connect(pInputLine, &CInputLine::InputEntered, pOutputView, &CConsoleView::WriteInput);
    connect(pCodeEditor, &CCodeEditor::InputEntered, pOutputView, &CConsoleView::WriteInput);
    connect(pFilesView, &CFilesView::FileSelected, pCodeEditor, &CCodeEditor::OpenFile);

    //This one is for startup dock sizes
    resizeDocks({dock1, dock2}, {400, 1000}, Qt::Horizontal);

    resize(1400, 800);

}

MainWindow::~MainWindow()
{
}

