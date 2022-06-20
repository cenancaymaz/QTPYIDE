#include "mainwindow.h"

#include "editor/editor.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    CEditor* p_editor = new CEditor(this);
    setCentralWidget(p_editor);
}

MainWindow::~MainWindow()
{
}

