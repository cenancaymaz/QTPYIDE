#include "line_number_area.h"



CLineNumberArea::CLineNumberArea(CSingleEditor *editor) : QWidget(editor) {
    codeEditor = editor;


}

QSize CLineNumberArea::sizeHint() const {
    return QSize(codeEditor->lineNumberAreaWidth(), 0);
}

void CLineNumberArea::paintEvent(QPaintEvent *event) {
    codeEditor->lineNumberAreaPaintEvent(event);
}
