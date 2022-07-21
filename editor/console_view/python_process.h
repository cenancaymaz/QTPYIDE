#ifndef CPYTHONPROCESS_H
#define CPYTHONPROCESS_H

#include <QProcess>

class CPythonProcess : public QProcess
{
    Q_OBJECT
public:
    explicit CPythonProcess(QObject *parent = nullptr);

    ~CPythonProcess();

    void StartProcess(QString Script = "");

    void WriteInp(QString Inp);

    void ControlSyntax(QString Script);

private slots:

    void ReadStdOut();

    void ReadErrOut();

    void Deneme();

signals:

    void OnStdOut(QString);

    void OnErrOut(QString);


};

#endif // CPYTHONPROCESS_H
