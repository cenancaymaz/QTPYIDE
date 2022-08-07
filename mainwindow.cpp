#include "mainwindow.h"

#include <QtWidgets>
#include <QDebug>

#include "util/startup_settings.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    SetAppTheme();

    setDockNestingEnabled(true);


    QDockWidget *dock0 = new QDockWidget(this);
    dock0->setAllowedAreas(Qt::TopDockWidgetArea);
    pCodeManager = new CCodeManager(dock0);
    dock0->setWidget(pCodeManager);
    dock0->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::TopDockWidgetArea, dock0, Qt::Horizontal);
    mDocks.insert(0, dock0);


    QDockWidget *dock1 = new QDockWidget(tr("Files"),this);
    dock1->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pFilesView = new CFilesView(dock1);
    pCodeManager->WorkingDirChanged(pFilesView->GetWorkingDir());
    pCodeManager->CurrentFilePathChanged(pFilesView->GetWorkingDir());
    connect(pFilesView, &CFilesView::WorkingDirChanged, pCodeManager, &CCodeManager::WorkingDirChanged);
    dock1->setWidget(pFilesView);
    dock1->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::LeftDockWidgetArea, dock1);
    mDocks.insert(1, dock1);



    QDockWidget *dock2 = new QDockWidget(tr("Code Editor"),this);
    dock2->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pCodeEditor = new CCodeEditor(dock2);
    connect(pCodeEditor, &CCodeEditor::CurrentTabPath, pCodeManager, &CCodeManager::CurrentFilePathChanged);
    dock2->setWidget(pCodeEditor);
    dock2->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    dock2->setTitleBarWidget(0);
    addDockWidget(Qt::RightDockWidgetArea, dock2);
    mDocks.insert(2, dock2);

    QDockWidget *dock3 = new QDockWidget(tr("Console"),this);
    dock3->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    pOutputView = new CConsoleView(pFilesView->GetWorkingPath(), dock3);
    connect(pFilesView, &CFilesView::WorkingPathChanged, pOutputView, &CConsoleView::WorkingPathChanged);
    dock3->setWidget(pOutputView);
    dock3->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
    addDockWidget(Qt::BottomDockWidgetArea, dock3, Qt::Vertical);
    mDocks.insert(3, dock3);


    //This had to be here, otherwise docking mechanism could not work properly
    QWidget* p_widget = new QWidget(this);
    p_widget->setFixedWidth(0);
    setCentralWidget(p_widget);


    QDockWidget *dock4 = new QDockWidget(this);
    dock4->setAllowedAreas(Qt::BottomDockWidgetArea);
    QLabel* p_status = new QLabel("Status", dock4);
    p_status->setFixedHeight(50);
    dock4->setWidget(p_status);
    dock4->setFeatures(QDockWidget::NoDockWidgetFeatures);
    addDockWidget(Qt::BottomDockWidgetArea, dock4, Qt::Vertical);
    mDocks.insert(4, dock4);


    //connect code manager's buttons to the code editor
    connect(pCodeManager, &CCodeManager::SaveFile, pCodeEditor, &CCodeEditor::SaveFile);
    connect(pCodeManager, &CCodeManager::SaveAsFile, pCodeEditor, &CCodeEditor::SaveAsFile);
    connect(pCodeManager, &CCodeManager::SendInput, pCodeEditor, &CCodeEditor::SendInput);
    connect(pCodeManager, &CCodeManager::SendSelected, pCodeEditor, &CCodeEditor::SendSelected);

    //connect code editor's button enabling situation to code manager
    connect(pCodeEditor, &CCodeEditor::EnableButtons, pCodeManager, &CCodeManager::EnableButtons);

    //connect input entered of code editor to console view's input running
    connect(pCodeEditor, &CCodeEditor::InputEntered, pOutputView, &CConsoleView::WriteInput);

    //connect files view's file selection to the code editor
    connect(pFilesView, &CFilesView::FileSelected, pCodeEditor, &CCodeEditor::OpenFile);

    //This one is for startup dock sizes
    //Somehow, we can't use the event handler because if we use, we lost splitter of the docks.
    //Therefore, we use singleshot timer with lambda expression
    QTimer::singleShot(400, this, [this]() {resizeDocks({this->mDocks[1], this->mDocks[2]}, {400, this->width() - 400}, Qt::Horizontal); } );


}

MainWindow::~MainWindow()
{
    CStartupSettings* p_set = GetStartupSettings();
    delete p_set;

}

void MainWindow::SetAppTheme()
{
    CStartupSettings* p_set = GetStartupSettings();

    qApp->setStyle(QStyleFactory::create("Fusion"));

    QPalette theme_palette;

    theme_palette.setColor(QPalette::Window, p_set->mColors[0]);
    theme_palette.setColor(QPalette::WindowText, QColor(p_set->mColors[2]));

    theme_palette.setColor(QPalette::Base, QColor(p_set->mColors[3]));
    theme_palette.setColor(QPalette::AlternateBase, p_set->mColors[0]);
    theme_palette.setColor(QPalette::ToolTipBase, p_set->mColors[4]);
    theme_palette.setColor(QPalette::ToolTipText, p_set->mColors[4]);

    theme_palette.setColor(QPalette::Text, QColor(p_set->mColors[2]));
    theme_palette.setColor(QPalette::Disabled, QPalette::Text, p_set->mColors[1]);
    theme_palette.setColor(QPalette::Button, p_set->mColors[0]);
    theme_palette.setColor(QPalette::ButtonText, QColor(p_set->mColors[5]));
    theme_palette.setColor(QPalette::Disabled, QPalette::ButtonText, p_set->mColors[1]);
    theme_palette.setColor(QPalette::BrightText, p_set->mColors[6]);
    theme_palette.setColor(QPalette::Link, QColor(p_set->mColors[7]));

    theme_palette.setColor(QPalette::Highlight, QColor(p_set->mColors[18]));
    theme_palette.setColor(QPalette::HighlightedText, QColor(p_set->mColors[2]));
    theme_palette.setColor(QPalette::Disabled, QPalette::HighlightedText, p_set->mColors[1]);

    qApp->setPalette(theme_palette);

    //qApp->setStyleSheet("QToolTip { color: #ffffff; background-color: #2aa198; border: 1px solid white; }");
    qApp->setStyleSheet(QString("QToolTip { color: %1; background-color: %2; border: 1px solid white; }")
                        .arg(p_set->mColors[4], p_set->mColors[9]));

}

