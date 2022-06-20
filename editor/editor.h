#ifndef CEDITOR_H
#define CEDITOR_H

#include <QWidget>

#include "history_view.h"
#include "code_editor.h"
#include "input_line.h"


class CEditor : public QWidget
{
    Q_OBJECT
public:
    explicit CEditor(QWidget *parent = nullptr);

private:

    CHistoryView *pHistoryView;
    CCodeEditor *pCodeEditor;
    CInputLine *pInputLine;

private slots:

    void InputEntered(QString text);


signals:

};

#endif // CEDITOR_H
