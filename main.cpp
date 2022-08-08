#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QSettings>
#include <QStyleFactory>

int countLines(QString path){
    QFile f(path);
    int cnt = 0;

    if(f.open(QIODevice::ReadOnly | QIODevice::Text)){
        QTextStream read(&f);
        while(!read.atEnd()){
            read.readLine();
            cnt++;
        }
    }
    f.close();
    return cnt;
}

int parseDir(QString path){
    int cnt = 0;
    QDir dir(path);
    QStringList dirs = dir.entryList(QDir::AllDirs |QDir::NoDotAndDotDot);
    QStringList file = dir.entryList(QDir::Files);
    for(QString dir : dirs){
            cnt += parseDir(path + "/"+dir);
    }

    for(QString s : file){
        if(s.splitRef('.').last() == "h" || s.splitRef('.').last() == "cpp")
            cnt += countLines(path + "/"+s);
    }

    return cnt;
}





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //Project line counter
    int count = 0;
    count += parseDir("E:\\DEV\\QTPYIDE\\QTPYIDE\\");
    QTextStream out(stdout);
    out << "Lines in project: " << a.arguments().last() << ": "<< endl <<count << endl;


    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "QtPyIDE_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            a.installTranslator(&translator);
            break;
        }
    }

    //to change all apps font size
    QFont font = a.font();
    font.setPointSize(font.pointSize() + 3);
    a.setFont(font);

    MainWindow w;
    w.showMaximized();

    //w.show();
    return a.exec();
}
