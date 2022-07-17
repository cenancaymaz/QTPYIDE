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
    mDocks.insert(0, dock1);



    QDockWidget *dock2 = new QDockWidget(tr("Code Editor"),this);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pCodeEditor = new CCodeEditor(dock2);
    dock2->setWidget(pCodeEditor);
    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock2->setTitleBarWidget(0);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    mDocks.insert(1, dock2);

    QDockWidget *dock3 = new QDockWidget(tr("Console"),this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pOutputView = new CConsoleView(dock3);
    dock3->setWidget(pOutputView);
    dock3->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, dock3, Qt::Vertical);
    mDocks.insert(2, dock3);


    QWidget* p_widget = new QWidget(this);
    p_widget->setFixedWidth(0);
    setCentralWidget(p_widget);


    QDockWidget *dock4 = new QDockWidget(this);
    dock4->setAllowedAreas(Qt::BottomDockWidgetArea);
    QLabel* p_status = new QLabel("Status", dock4);
    dock4->setWidget(p_status);
    dock4->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dock4, Qt::Vertical);
    mDocks.insert(3, dock4);


    connect(pCodeEditor, &CCodeEditor::InputEntered, pOutputView, &CConsoleView::WriteInput);
    connect(pFilesView, &CFilesView::FileSelected, pCodeEditor, &CCodeEditor::OpenFile);


    //This one is for startup dock sizes
    //Somehow, we can't use the event handler because if we use, we lost splitter of the docks.
    //Therefore, we use singleshot timer with lambda expression
    QTimer::singleShot(200, [&]() {resizeDocks({mDocks[0], mDocks[1]}, {400, width() - 400}, Qt::Horizontal); } );

}

MainWindow::~MainWindow()
{
}

