#include "console_view.h"

#include <QtWidgets>

#include "../../util/startup_settings.h"


#include "../../util/python_process.h"

CConsoleView::CConsoleView(QString InitialPath, QWidget *parent)
    : QFrame{parent}
{

    mWorkingPath = InitialPath;

    CStartupSettings* p_set = CStartupSettings::GetInstance();;

    setStyleSheet(QString("CConsoleView{ border: 1px solid %1; }").arg(p_set->mColors[10]));

    pTabWidget = new QTabWidget(this);
    pTabWidget->setMovable(true);

    p_set->SettoDefaultFontSize(pTabWidget);

    //For controlling tab movement
    connect(pTabWidget->tabBar(), &QTabBar::tabMoved, this, &CConsoleView::TabPosControl);


    LatestTabNo = 0;


    //For new Tab Button
    QToolButton *tb = new QToolButton();
    tb->setText("+");
    tb->setStyleSheet("border : none");

    p_set->SettoDefaultFontSize(tb);

    connect(tb, &QToolButton::clicked, this, &CConsoleView::AddEmptyTab);

    //Create a dummy tab and attach the tool button to it
    pTabWidget->addTab(new QLabel("You can open a console by pressing <b>\"+\"</b>"), QString());
    pTabWidget->setTabEnabled(0, false);
    pTabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);

    //The First Console
    AddEmptyTab();

    //Create Input Line and connect it to the input mechanism

    pInputLine = new CInputLine(this);
    connect(pInputLine, &CInputLine::InputSent, this, &CConsoleView::WriteInput);


    //Widget layouting
    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(3, 3, 3, 3);
    vl->addWidget(pTabWidget);
    vl->addWidget(pInputLine);
}

void CConsoleView::WriteInput(QString text)
{
    //Take selected tab
    CSingleConsole* p_single_console = qobject_cast<CSingleConsole*>(pTabWidget->currentWidget());

    if(!p_single_console){//There is no console in the tab widget

        //Create a new tab
        p_single_console = AddTab();

    }

    //Send input to the console
    p_single_console->WriteInput(text);

    //Add it to the history stack of the input line
    pInputLine->AddToHistoryStack(text);

}

void CConsoleView::WorkingPathChanged(QString Path)
{
    mWorkingPath = Path;
}

void CConsoleView::AddEmptyTab()
{
    CSingleConsole* p_single_console = AddTab();
    p_single_console->StartProcess(mWorkingPath);
}


QTextEdit *CConsoleView::CreateAConsole()
{
    //Create an empty console
    QTextEdit* p_text_edit = new QTextEdit(this);
    p_text_edit->setReadOnly(true);

    return p_text_edit;
}

CSingleConsole *CConsoleView::AddTab()
{
    CSingleConsole *p_console = new CSingleConsole(this);
    QString tabName = QString("Console%1").arg(++LatestTabNo);
    pTabWidget->insertTab(pTabWidget->count() - 1, p_console, tabName);

    //Insert tab tool tip to the tab
    pTabWidget->setTabToolTip(pTabWidget->count() - 2, "Working Dir: " + mWorkingPath);

    //For Close Button
    QToolButton *cb = new QToolButton();
    cb->setText("x");
    cb->setStyleSheet("border : none");

    CStartupSettings* p_set = CStartupSettings::GetInstance();;
    p_set->SettoDefaultFontSize(cb);

    connect(cb, &QToolButton::clicked, this, &CConsoleView::CloseTab);

    pTabWidget->tabBar()->setTabButton(pTabWidget->count() - 2, QTabBar::RightSide, cb);


    pTabWidget->setCurrentIndex(pTabWidget->count() - 2);

    return p_console;
}

void CConsoleView::CloseTab()
{
    QToolButton* p_tool_button = qobject_cast<QToolButton*>(sender());
    QTabBar* p_tab_bar = qobject_cast<QTabBar*>(sender()->parent());

    int closed_ind = p_tab_bar->tabAt(p_tool_button->pos());

    pTabWidget->removeTab(closed_ind);


    if(closed_ind > 0){

        pTabWidget->setCurrentIndex(closed_ind - 1);
    }


}

void CConsoleView::TabPosControl()
{
    //Take the last tab class name
    QString last_tab_class(pTabWidget->widget(pTabWidget->count() - 1)->metaObject()->className());

    if(last_tab_class == "CSingleConsole"){//it is a console!

        pTabWidget->tabBar()->moveTab(pTabWidget->count() - 2, pTabWidget->count() - 1); //Move new tab button to end

    }
}
