#ifndef CSINGLEEDITOR_H
#define CSINGLEEDITOR_H

#include <QTextEdit>

class CSingleEditor : public QTextEdit
{
    Q_OBJECT
public:

    explicit CSingleEditor(QWidget *parent = 0);

    void SetCurrentContent(QString Content);
    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

signals:

    void ContentChanged();

public slots:
    void resizeEvent(QResizeEvent *e);


private slots:

    void ControlContentChange();

    void updateLineNumberAreaWidth(int newBlockCount);
    void updateLineNumberArea(QRectF /*rect_f*/);
    void updateLineNumberArea(int /*slider_pos*/);
    void updateLineNumberArea();


private:

    QWidget *lineNumberArea;
    QString mCurrentContent;


};

#endif // CSINGLEEDITOR_H
