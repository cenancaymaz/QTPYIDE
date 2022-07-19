#ifndef CLINENUMBERAREA_H
#define CLINENUMBERAREA_H

#include <QtWidgets>

class CLineNumberArea : public QWidget
{
    Q_OBJECT

    public:
        CLineNumberArea(QTextEdit *editor);

        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        QTextEdit *codeEditor;

};

#endif // CLINENUMBERAREA_H
