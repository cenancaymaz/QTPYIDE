#ifndef CCONSOLEVIEW_H
#define CCONSOLEVIEW_H

#include <QGroupBox>
#include <QTextEdit>

class CConsoleView : public QGroupBox
{
    Q_OBJECT
public:
    explicit CConsoleView(QWidget *parent = nullptr);

    void WriteInput(QString text);

private:
    QTextEdit* pTextEdit;

signals:

};

#endif // CCONSOLEVIEW_H
