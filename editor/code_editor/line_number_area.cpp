#include "line_number_area.h"

#include "text_edit_highlighter.h"

CLineNumberArea::CLineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;


}

QSize CLineNumberArea::sizeHint() const {
    return QSize(((CTextEditHighlighter *)codeEditor)->lineNumberAreaWidth(), 0);
}

void CLineNumberArea::paintEvent(QPaintEvent *event) {
    ((CTextEditHighlighter *)codeEditor)->lineNumberAreaPaintEvent(event);
}
