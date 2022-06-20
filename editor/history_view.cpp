#include "history_view.h"

#include <QDateTime>

#include <QApplication>

CHistoryView::CHistoryView(QWidget *parent)
    : QTextEdit{parent}
{
    setReadOnly(true);
}

void CHistoryView::WriteHistory(QString text)
{
    QString date_text = QDateTime::currentDateTime().toString("dd.MM.yyyy hh:mm") + " : ";

    QPalette palette = qApp->palette();

    setTextColor(palette.color(QPalette::Disabled, QPalette::Text));

    insertPlainText(date_text);

    setTextColor(qApp->palette().text().color());
    insertPlainText(text);
    insertPlainText("\n");
}
