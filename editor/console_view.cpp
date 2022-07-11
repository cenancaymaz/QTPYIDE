#include "console_view.h"

#include <QBoxLayout>
#include <QDateTime>
#include <QScrollBar>

#include <QApplication>

CConsoleView::CConsoleView(QWidget *parent)
    : QGroupBox{parent}
{
    setTitle(tr("Console"));
    pTextEdit = new QTextEdit(this);
    pTextEdit->setReadOnly(true);

    QVBoxLayout *vl = new QVBoxLayout(this);

    vl->addWidget(pTextEdit);
}

void CConsoleView::WriteInput(QString text)
{
    QString date_text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") + " : ";

    QPalette palette = qApp->palette();

    pTextEdit->setTextColor(palette.color(QPalette::Disabled, QPalette::Text));

    pTextEdit->insertPlainText(date_text);

    //When run selected, this line does not work
    text.replace("\n", "\n                           ");//for aligning to date_text
    pTextEdit->setTextColor(qApp->palette().text().color());
    pTextEdit->insertPlainText(text);
    pTextEdit->insertPlainText("\n");

    pTextEdit->verticalScrollBar()->setValue(pTextEdit->verticalScrollBar()->maximum());
}

