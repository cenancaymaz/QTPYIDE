#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "editor/console_view/console_view.h"
#include "editor/code_editor/code_editor.h"
#include "editor/files_view.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    //bool event(QEvent* ev) override;

private:

    CConsoleView *pOutputView;
    CCodeEditor *pCodeEditor;
    CFilesView *pFilesView;

    QList<QDockWidget*> mDocks;

    void SetAppTheme();



};
#endif // MAINWINDOW_H
