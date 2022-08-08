#ifndef CLINENUMBERAREA_H
#define CLINENUMBERAREA_H

#include <QWidget>
#include "single_editor.h"

class CLineNumberArea : public QWidget
{
    Q_OBJECT

    public:
        CLineNumberArea(CSingleEditor *editor);

        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *event);

    private:
        CSingleEditor *codeEditor;

};

#endif // CLINENUMBERAREA_H
