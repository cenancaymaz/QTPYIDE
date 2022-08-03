#ifndef CLINENUMBERAREA_H
#define CLINENUMBERAREA_H

#include <QWidget>
#include <QTextEdit>
#include <QPlainTextEdit>

class CLineNumberArea : public QWidget
{
    Q_OBJECT

    public:
        CLineNumberArea(QPlainTextEdit *editor);

        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        QPlainTextEdit *codeEditor;

};

#endif // CLINENUMBERAREA_H
