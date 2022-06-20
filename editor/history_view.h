#ifndef CHISTORYVIEW_H
#define CHISTORYVIEW_H

#include <QTextEdit>

class CHistoryView : public QTextEdit
{
    Q_OBJECT
public:
    explicit CHistoryView(QWidget *parent = nullptr);

    void WriteHistory(QString text);

signals:

};

#endif // CHISTORYVIEW_H
