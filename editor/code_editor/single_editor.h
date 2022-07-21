#ifndef CSINGLEEDITOR_H
#define CSINGLEEDITOR_H

#include <QTextEdit>

class CSingleEditor : public QTextEdit
{
    Q_OBJECT
public:

    explicit CSingleEditor(QWidget *parent = 0);

    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

signals:


public slots:
    void resizeEvent(QResizeEvent *e);


private slots:

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();


private:

    QWidget *lineNumberArea;


};

#endif // CSINGLEEDITOR_H
