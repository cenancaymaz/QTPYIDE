#include "mainwindow.h"

#include <QApplication>
#include <QLocale>
#include <QTranslator>

#include <QSettings>
#include <QStyleFactory>


int main(int argc, char *argv[])
{

    QApplication a(argc, argv);

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
