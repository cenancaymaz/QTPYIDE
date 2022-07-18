#include "console_view.h"

#include <QtWidgets>

#include "../startup_settings.h"

CConsoleView::CConsoleView(QWidget *parent)
    : QFrame{parent}
{
    CStartupSettings* p_set = GetStartupSettings();

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

    connect(tb, &QToolButton::clicked, this, &CConsoleView::AddTab);

    //Create a dummy tab and attach the tool button to it
    pTabWidget->addTab(new QLabel("You can open a console by pressing <b>\"+\"</b>"), QString());
    pTabWidget->setTabEnabled(0, false);
    pTabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);

    //The First Console
    AddTab();

    pInputEdit = new QLineEdit(this);

    connect(pInputEdit, &QLineEdit::returnPressed, this, &CConsoleView::SendInputFromLine);

    pSendButton = new QPushButton(">", this);
    pSendButton->setFixedWidth(30);
    connect(pSendButton, &QPushButton::clicked, this, &CConsoleView::SendInputFromLine);



    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->addWidget(pTabWidget);

    QHBoxLayout* hl = new QHBoxLayout();

    hl->addWidget(new QLabel(tr("In:"), this));
    hl->addWidget(pInputEdit);
    hl->addWidget(pSendButton);

    vl->addLayout(hl);
}

void CConsoleView::WriteInput(QString text)
{
    CStartupSettings* p_set = GetStartupSettings();

    //Take selected tab
    QTextEdit* p_text_edit = qobject_cast<QTextEdit*>(pTabWidget->currentWidget());

    if(!p_text_edit){//There is no text edit in the tab widget

        //Create a new tab
        p_text_edit = AddTab();

    }

    //Set cursor to the end of the edit
    QTextCursor new_cursor = p_text_edit->textCursor();
    new_cursor.movePosition(QTextCursor::End);
    p_text_edit->setTextCursor(new_cursor);

    p_text_edit->setTextColor(p_set->mColors[6]);
    p_text_edit->insertPlainText("> ");

    //BUG - When using "run selected", this line does not work
    text.replace("\n", "\n   ");//for aligning to input arrow

    //Add the input text with appropriate color
    p_text_edit->setTextColor(p_set->mColors[2]);
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

QTextEdit* CConsoleView::AddTab()
{
    QTextEdit *tab = CreateAConsole();
    QString tabName = QString("Console%1").arg(++LatestTabNo);
    pTabWidget->insertTab(pTabWidget->count() - 1, tab, tabName);


    //For Close Button
    QToolButton *cb = new QToolButton();
    cb->setText("x");
    cb->setStyleSheet("border : none");

    CStartupSettings* p_set = GetStartupSettings();
    p_set->SettoDefaultFontSize(cb);

    connect(cb, &QToolButton::clicked, this, &CConsoleView::CloseTab);

    pTabWidget->tabBar()->setTabButton(pTabWidget->count() - 2, QTabBar::RightSide, cb);


    pTabWidget->setCurrentIndex(pTabWidget->count() - 2);

    return tab;
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

    if(last_tab_class == "QTextEdit"){//it is a console!

        pTabWidget->tabBar()->moveTab(pTabWidget->count() - 2, pTabWidget->count() - 1); //Move new tab button to end

    }
}

void CConsoleView::SendInputFromLine()
{
    pInputEdit->setFocus();
    if(!pInputEdit->text().isEmpty()){
        WriteInput(pInputEdit->text());
        pInputEdit->clear();
    }
}

