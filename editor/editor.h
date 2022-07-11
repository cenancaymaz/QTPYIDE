#ifndef CEDITOR_H
#define CEDITOR_H

#include <QWidget>

#include "console_view.h"
#include "code_editor.h"
#include "input_line.h"


class CEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CEditor(QWidget *parent = nullptr);

private:

    CConsoleView *pOutputView;
    CCodeEditor *pCodeEditor;
    CInputLine *pInputLine;

private slots:

    void InputEntered(QString text);


signals:

};

#endif // CEDITOR_H
