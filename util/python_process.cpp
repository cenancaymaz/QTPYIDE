#include "python_process.h"
#include <QDebug>
#include <QTimer>
#include <QThread>

CPythonProcess::CPythonProcess(QObject *parent)
    : QProcess{parent}
{
    //Connect standart output and standart error channels
    connect(this, &CPythonProcess::readyReadStandardOutput, this, &CPythonProcess::ReadStdOut);
    connect(this, &CPythonProcess::readyReadStandardError, this, &CPythonProcess::ReadErrOut);

}

CPythonProcess::~CPythonProcess()
{
    //First, exit from python
    WriteInp("exit()");
    //Close the writing channel
    closeWriteChannel();
    //Kill the process
    kill();
    //Wait for the termination of python process
    waitForFinished();
}

void CPythonProcess::StartProcess(QString Path, QString Script)
{
    //This opens a python terminal. Inputs are from stdin and outputs are from stdout and stderr
    //If there is a script in here, firstly the script is run then terminal resumes
    //-i is for forcing prompts after script
    //-q is for clearing interactive startup

    setWorkingDirectory(Path);
    start(QString("python -i -q %1").arg(Script));
}

void CPythonProcess::WriteInp(QString Inp)
{

    QStringList lines = Inp.split("\n");

    foreach(QString line, lines){

        //Write the input
        write(QString(line + "\n").toLatin1());
        //Wait for write to finish
        waitForBytesWritten();
    }

    //Write a new line for RunSelected
    write(QString("\n").toLatin1());
    //Wait for write to finish
    waitForBytesWritten();

}

void CPythonProcess::ControlSyntax(QString Script)
{
    start(QString("python -m py_compile %1").arg(Script));
    waitForFinished();
}

QString CPythonProcess::GetPythonVersion()
{

    disconnect(this, &CPythonProcess::readyReadStandardOutput, this, &CPythonProcess::ReadStdOut);

    start(QString("python -i -q"));//.arg(command));

    WriteInp("import sys");
    WriteInp("sys.version_info");

    waitForReadyRead();

    QString rep = readAllStandardOutput();

    QString maj = rep.mid(rep.indexOf("major=") + 6, rep.indexOf("minor=") - rep.indexOf("major=") - 8);
    QString min = rep.mid(rep.indexOf("minor=") + 6, rep.indexOf("micro=") - rep.indexOf("minor=") - 8);

    return maj + min;
}

void CPythonProcess::ReadStdOut()
{
    //for reading std out line by line
    setReadChannel(QProcess::StandardOutput);

    while(canReadLine()){
//        QString reply = readLine().trimmed();
//        qDebug()<<reply;
//        emit OnStdOut(reply);
        emit OnStdOut(readLine().trimmed());
    }
}

void CPythonProcess::ReadErrOut()
{
    //for reading std err line by line
    setReadChannel(QProcess::StandardError);
    while(canReadLine()){
        emit OnErrOut(readLine().trimmed());
    }
}
