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

void CPythonProcess::StartProcess(QString Script)
{
    //This opens a python terminal. Inputs are from stdin and outputs are from stdout and stderr
    //If there is a script in here, firstly the script is run then terminal resumes
    //-i is for forcing prompts after script
    //-q is for clearing interactive startup
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
    qDebug()<<"Control happening";
    start(QString("python -m py_compile %1").arg(Script));
}

void CPythonProcess::ReadStdOut()
{
    QString line;
    //for reading std out line by line
    setReadChannel(QProcess::StandardOutput);
    while(canReadLine()){
        line = readLine().trimmed();
        qDebug()<<"Std Out: "<<line;
        emit OnStdOut(line);
        //emit OnStdOut(readLine().trimmed());
    }
}

void CPythonProcess::ReadErrOut()
{
    QString line;
    //for reading std err line by line
    setReadChannel(QProcess::StandardError);
    while(canReadLine()){
        line = readLine().trimmed();
        qDebug()<<"Std Out: "<<line;
        emit OnStdOut(line);
        //emit OnErrOut(readLine().trimmed());
    }
}

void CPythonProcess::Deneme()
{
    qDebug()<<"deneme";

//    this->write("python.exe\r\n");
//    this->waitForBytesWritten();
//    this->closeWriteChannel();



//    this->write("a = 5\r\n");
//    this->waitForBytesWritten();
//    this->closeWriteChannel();

//    QThread::msleep(500);

    //this->setProcessChannelMode(QProcess::MergedChannels);


//    this->write("python \n");
//    this->waitForBytesWritten();
//    qDebug()<<"deneme2";

      WriteInp("print_hi('world')");
//    WriteInp("print (\"sad\")");
//    qDebug()<<"deneme2";
//    WriteInp("print (\"dad\")");
//    qDebug()<<"deneme3";
//    WriteInp("a = 5");
//    qDebug()<<"deneme4";
//    WriteInp("a");
//    qDebug()<<"deneme5";
//    WriteInp("ads");
//    qDebug()<<"deneme6";
//    WriteInp("print(a)");
//    qDebug()<<"deneme7";



//    this->write("print (\"sad\") \n");
//    this->waitForBytesWritten();
//    //this->closeWriteChannel();
//    qDebug()<<"deneme3";


//    this->write("print (\"dad\") \n");
//    this->waitForBytesWritten();
//    qDebug()<<"deneme4";

    //this->waitForReadyRead(-1);

    //qDebug()<<this->readAll();

}
