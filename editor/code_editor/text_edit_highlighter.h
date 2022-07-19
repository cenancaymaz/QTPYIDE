#ifndef CTEXTEDITHIGHLIGHTER_H
#define CTEXTEDITHIGHLIGHTER_H

#include <QtWidgets>

class CTextEditHighlighter : public QTextEdit
{
    Q_OBJECT
public:

    explicit CTextEditHighlighter(QWidget *parent = 0);

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

#endif // CTEXTEDITHIGHLIGHTER_H
