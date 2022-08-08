#ifndef CSINGLECONSOLE_H
#define CSINGLECONSOLE_H

#include <QtWidgets>
#include "../../util/python_process.h"

class CSingleConsole : public QTextEdit
{
    Q_OBJECT
public:
    explicit CSingleConsole(QWidget *parent = nullptr);

    void StartProcess(QString Path, QString Script = "");

    void WriteInput(QString Inp);

private:
    CPythonProcess* pPythonProcess;

    QColor mInputIndCol;

    void InsertInputInd();

    void DeleteLastLine();

private slots:

    void OnStdOut(QString Out);

    void OnErrOut(QString Err);

signals:

};

#endif // CSINGLECONSOLE_H
