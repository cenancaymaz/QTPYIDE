#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "editor/console_view.h"
#include "editor/code_editor.h"
#include "editor/input_line.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
private:

    CConsoleView *pOutputView;
    CCodeEditor *pCodeEditor;
    CInputLine *pInputLine;

};
#endif // MAINWINDOW_H
