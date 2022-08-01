#ifndef CCONSOLEVIEW_H
#define CCONSOLEVIEW_H

#include <QtWidgets>
#include "single_console.h"

class CConsoleView : public QFrame
{
    Q_OBJECT
public:
    explicit CConsoleView(QString InitialPath, QWidget *parent = nullptr);

private:

    QString mWorkingPath;

    int LatestTabNo;

    QTabWidget* pTabWidget;

    QTextEdit* CreateAConsole();

    QLineEdit* pInputEdit;

    QPushButton* pSendButton;

public slots:

    void WriteInput(QString text);

    void WorkingPathChanged(QString Path);

private slots:

    void AddEmptyTab();

    CSingleConsole* AddTab();

    void CloseTab();

    void TabPosControl();

    void SendInputFromLine();


};

#endif // CCONSOLEVIEW_H
