#include "line_number_area.h"

#include "single_editor.h"

CLineNumberArea::CLineNumberArea(QTextEdit *editor) : QWidget(editor) {
    codeEditor = editor;


}

QSize CLineNumberArea::sizeHint() const {
    return QSize(((CSingleEditor *)codeEditor)->lineNumberAreaWidth(), 0);
}

void CLineNumberArea::paintEvent(QPaintEvent *event) {
    ((CSingleEditor *)codeEditor)->lineNumberAreaPaintEvent(event);
}
