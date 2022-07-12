#include "console_view.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QScrollBar>
#include <QToolButton>
#include <QTabBar>
#include <QLabel>
#include <QTextCursor>
#include <QDebug>
#include <QSettings>

#include <QApplication>

CConsoleView::CConsoleView(QWidget *parent)
    : QFrame{parent}
{
    //setTitle(tr("Console"));

    //This part is for Windows Dark Theme users
    #ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
        if(settings.value("AppsUseLightTheme")==0){
           setStyleSheet("CConsoleView{ border: 1px solid ""#FF4C4C4C""; }");
        }else{

            setStyleSheet("CConsoleView{ border: 1px solid ligthgray; }");
        }
    #endif

    pTabWidget = new QTabWidget(this);
    pTabWidget->setMovable(true);

    //For controlling tab movement
    connect(pTabWidget->tabBar(), &QTabBar::tabMoved, this, &CConsoleView::TabPosControl);


    LatestTabNo = 0;


    //For new Tab Button
    QToolButton *tb = new QToolButton();
    tb->setText("+");
    tb->setStyleSheet("border : none");
    connect(tb, &QToolButton::clicked, this, &CConsoleView::AddTab);

    //Create a dummy tab and attach the tool button to it
    pTabWidget->addTab(new QLabel("You can open a console by pressing <b>\"+\"</b>"), QString());
    pTabWidget->setTabEnabled(0, false);
    pTabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);



    //The First Console
    AddTab();
    //pTabWidget->addTab(AddAConsole(), tr("Console 1"));


    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->addWidget(pTabWidget);
}

void CConsoleView::WriteInput(QString text)
{
    //Take selected tab
    QTextEdit* p_text_edit = qobject_cast<QTextEdit*>(pTabWidget->currentWidget());

    //Set cursor to the end of the edit
    QTextCursor new_cursor = p_text_edit->textCursor();
    new_cursor.movePosition(QTextCursor::End);
    p_text_edit->setTextCursor(new_cursor);

    //Find color for input arrow for the mode and add the input arrow
    QPalette palette = qApp->palette();
    p_text_edit->setTextColor(palette.color(QPalette::Disabled, QPalette::Text));
    p_text_edit->insertPlainText("> ");

    //BUG - When using "run selected", this line does not work
    text.replace("\n", "\n   ");//for aligning to input arrow

    //Add the input text with appropriate color
    p_text_edit->setTextColor(qApp->palette().text().color());
    p_text_edit->insertPlainText(text);
    p_text_edit->insertPlainText("\n");

    //Set scrollbar to the bottom
    p_text_edit->verticalScrollBar()->setValue(p_text_edit->verticalScrollBar()->maximum());
}

QTextEdit *CConsoleView::CreateAConsole()
{
    //Create an empty console
    QTextEdit* p_text_edit = new QTextEdit(this);
    p_text_edit->setReadOnly(true);

    return p_text_edit;
}

void CConsoleView::AddTab()
{
    QTextEdit *tab = CreateAConsole();
    QString tabName = QString("Console%1").arg(++LatestTabNo);
    pTabWidget->insertTab(pTabWidget->count() - 1, tab, tabName);


    //For Close Button
    QToolButton *cb = new QToolButton();
    cb->setText("x");
    cb->setStyleSheet("border : none");
    connect(cb, &QToolButton::clicked, this, &CConsoleView::CloseTab);

    pTabWidget->tabBar()->setTabButton(pTabWidget->count() - 2, QTabBar::RightSide, cb);


    pTabWidget->setCurrentIndex(pTabWidget->count() - 2);
}

void CConsoleView::CloseTab()
{
    QToolButton* p_tool_button = qobject_cast<QToolButton*>(sender());
    QTabBar* p_tab_bar = qobject_cast<QTabBar*>(sender()->parent());

    int closed_ind = p_tab_bar->tabAt(p_tool_button->pos());

    pTabWidget->removeTab(closed_ind);
}

void CConsoleView::TabPosControl()
{
    //Take the last tab class name
    QString last_tab_class(pTabWidget->widget(pTabWidget->count() - 1)->metaObject()->className());

    if(last_tab_class == "QTextEdit"){//it is a console!

        pTabWidget->tabBar()->moveTab(pTabWidget->count() - 2, pTabWidget->count() - 1); //Move new tab button to end

    }
}

