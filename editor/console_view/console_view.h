#ifndef CCONSOLEVIEW_H
#define CCONSOLEVIEW_H

#include <QtWidgets>
#include "single_console.h"

class CConsoleView : public QFrame
{
    Q_OBJECT
public:
    explicit CConsoleView(QWidget *parent = nullptr);

private:

    int LatestTabNo;

    QTabWidget* pTabWidget;

    QTextEdit* CreateAConsole();

    QLineEdit* pInputEdit;

    QPushButton* pSendButton;

public slots:

    void WriteInput(QString text);

private slots:

    void AddEmptyTab();

    void AddScriptTab();

    CSingleConsole* AddTab();

    void CloseTab();

    void TabPosControl();

    void SendInputFromLine();


};

#endif // CCONSOLEVIEW_H
