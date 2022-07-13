#ifndef CCONSOLEVIEW_H
#define CCONSOLEVIEW_H

#include <QGroupBox>
#include <QTextEdit>

#include <QTabWidget>

class CConsoleView : public QFrame
{
    Q_OBJECT
public:
    explicit CConsoleView(QWidget *parent = nullptr);

private:
int LatestTabNo;


    QTabWidget* pTabWidget;

    QTextEdit* CreateAConsole();

public slots:

    void WriteInput(QString text);

private slots:

    QTextEdit* AddTab();

    void CloseTab();

    void TabPosControl();

signals:

};

#endif // CCONSOLEVIEW_H
