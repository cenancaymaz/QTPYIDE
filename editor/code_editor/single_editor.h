#ifndef CSINGLEEDITOR_H
#define CSINGLEEDITOR_H

#include <QTextEdit>
#include <QPlainTextEdit>

class CSingleEditor : public QTextEdit
{
    Q_OBJECT
public:

    explicit CSingleEditor(QWidget *parent = 0);

    void SetCurrentContent(QString Content);
    int getFirstVisibleBlockId();
    void lineNumberAreaPaintEvent(QPaintEvent *event);
    int lineNumberAreaWidth();

    void FindNext(QString Text);
    void FindPrev(QString Text);

    bool event(QEvent *event);

signals:

    void ContentChanged();
    void OpenSearchWidget(QString SelectedText);

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

    bool mTextFound;
    QString mSearchedText;

    void insertFromMimeData(const QMimeData* source );

    //Events
    void keyPressEvent(QKeyEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);
    void wheelEvent(QWheelEvent *e);



};

#endif // CSINGLEEDITOR_H
