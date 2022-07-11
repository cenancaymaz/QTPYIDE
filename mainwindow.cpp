#include "mainwindow.h"

#include "editor/editor.h"
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CEditor* p_editor = new CEditor(this);
    setCentralWidget(p_editor);

    resize(600, 600);
}

MainWindow::~MainWindow()
{
}

