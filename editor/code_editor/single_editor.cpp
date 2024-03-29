#include "single_editor.h"

#include "../../util/startup_settings.h"
#include "line_number_area.h"

#include <QtWidgets>


CSingleEditor::CSingleEditor(QWidget *parent) :
    QTextEdit(parent)
{
    setMouseTracking(true);

    mTextFound = false;
    mSearchedText = "";

    connect(this, &CSingleEditor::textChanged, this, &CSingleEditor::ControlContentChange);

    // Line numbers
    lineNumberArea = new CLineNumberArea(this);
    ///
    connect(this->document(), SIGNAL(blockCountChanged(int)), this, SLOT(updateLineNumberAreaWidth(int)));
    connect(this->verticalScrollBar(), SIGNAL(valueChanged(int)), this, SLOT(updateLineNumberArea/*_2*/(int)));
    connect(this, SIGNAL(textChanged()), this, SLOT(updateLineNumberArea()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(updateLineNumberArea()));
    ///
    updateLineNumberAreaWidth(0);
}

void CSingleEditor::SetCurrentContent(QString Content)
{
    setText(Content);
    //setText(Content);
    mCurrentContent = Content;
}

int CSingleEditor::lineNumberAreaWidth()
{
    int digits = 1;
    int max = qMax(1, this->document()->blockCount());
    while (max >= 10) {
        max /= 10;
        ++digits;
    }

    int space = 13 +  fontMetrics().width(QLatin1Char('9')) * (digits);

    return space;
}

void CSingleEditor::FindNext(QString Text)
{
    if(Text != mSearchedText){ //A new search is started

        mSearchedText = Text;
        mTextFound = false;
    }
    //Find the text
    if(find(Text)){

        mTextFound = true;

    }else{//not found

        if(mTextFound){//it has previously found, but not now
            //return cursor to the start of the document and search again
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::Start);
            setTextCursor(cursor);
            FindNext(Text);

        }else{
            //it has never be found. Do nothing
        }
    }

}

void CSingleEditor::FindPrev(QString Text)
{
    if(Text != mSearchedText){ //A new search is started

        mSearchedText = Text;
        mTextFound = false;
    }
    //Find the text
    if(find(Text, QTextDocument::FindBackward)){

        mTextFound = true;

    }else{//not found

        if(mTextFound){//it has previously found, but not now
            //return cursor to the start of the document and search again
            QTextCursor cursor = textCursor();
            cursor.movePosition(QTextCursor::End);
            setTextCursor(cursor);
            FindPrev(Text);

        }else{
            //it has never be found. Do nothing
        }
    }
}

bool CSingleEditor::event(QEvent *event)
{
    if(event->type() == QEvent::ToolTip){

        QHelpEvent* helpEvent = static_cast<QHelpEvent*>(event);

        QPoint pos = helpEvent->pos();
        pos.setX(pos.x() - viewportMargins().left());
        pos.setY(pos.y() - viewportMargins().top());
        QTextCursor cursor = cursorForPosition(pos);

        QVector<QTextLayout::FormatRange> fmts = cursor.block().layout()->formats();

        foreach(QTextLayout::FormatRange fmt, fmts){

            cursor.setPosition(fmt.start);
            cursor.setPosition(fmt.start + fmt.length, QTextCursor::KeepAnchor);

            QToolTip::showText(helpEvent->globalPos(), fmt.format.toolTip(), this, this->cursorRect(cursor));

        }
    }
    return QTextEdit::event(event);
}

void CSingleEditor::updateLineNumberAreaWidth(int /* newBlockCount */)
{
    setViewportMargins(lineNumberAreaWidth(), 0, 0, 0);
}


void CSingleEditor::updateLineNumberArea(QRectF /*rect_f*/)
{
    CSingleEditor::updateLineNumberArea();
}
void CSingleEditor::updateLineNumberArea(int /*slider_pos*/)
{
    CSingleEditor::updateLineNumberArea();
}
void CSingleEditor::updateLineNumberArea()
{
    /*
     * When the signal is emitted, the sliderPosition has been adjusted according to the action,
     * but the value has not yet been propagated (meaning the valueChanged() signal was not yet emitted),
     * and the visual display has not been updated. In slots connected to this signal you can thus safely
     * adjust any action by calling setSliderPosition() yourself, based on both the action and the
     * slider's value.
     */
    // Make sure the sliderPosition triggers one last time the valueChanged() signal with the actual value !!!!
    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    // Since "QTextEdit" does not have an "updateRequest(...)" signal, we chose
    // to grab the imformations from "sliderPosition()" and "contentsRect()".
    // See the necessary connections used (Class constructor implementation part).

    QRect rect =  this->contentsRect();
    lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    updateLineNumberAreaWidth(0);
    //----------
    int dy = this->verticalScrollBar()->sliderPosition();
    if (dy > -1) {
        lineNumberArea->scroll(0, dy);
    }

    // Addjust slider to alway see the number of the currently being edited line...
    int first_block_id = getFirstVisibleBlockId();
    if (first_block_id == 0 || this->textCursor().block().blockNumber() == first_block_id-1)
        this->verticalScrollBar()->setSliderPosition(dy-this->document()->documentMargin());

    QFont font = lineNumberArea->font();
    font.setPointSize(this->font().pointSize());
    lineNumberArea->setFont(font);

//    // Snap to first line (TODO...)
//    if (first_block_id > 0)
//    {
//        int slider_pos = this->verticalScrollBar()->sliderPosition();
//        int prev_block_height = (int) this->document()->documentLayout()->blockBoundingRect(this->document()->findBlockByNumber(first_block_id-1)).height();
//        if (dy <= this->document()->documentMargin() + prev_block_height)
//            this->verticalScrollBar()->setSliderPosition(slider_pos - (this->document()->documentMargin() + prev_block_height));
//    }

}

void CSingleEditor::insertFromMimeData(const QMimeData *source)
{
    QTextEdit::insertPlainText(source->text());
}

void CSingleEditor::keyPressEvent(QKeyEvent *e)
{
    //Capture CTRL-F for search widget
    if (e->key() == Qt::Key_F && (QApplication::keyboardModifiers() & Qt::ControlModifier))
    {
        emit OpenSearchWidget(textCursor().selectedText());
    }

    QTextEdit::keyPressEvent(e);
}

void CSingleEditor::contextMenuEvent(QContextMenuEvent *e)
{
    CStartupSettings* p_set = CStartupSettings::GetInstance();;

    //Take the standart menu
    QMenu* p_menu = createStandardContextMenu(e->pos());
    //Create a custom style for the menu
    QString menu_style = QString(
            "QMenu::item{"
                "color: %1;"
                "font: 15px;"
                "padding: 2px 25px 2px 20px;"
                "border: 1px solid transparent;"
            "}"
            "QMenu::item:disabled{"
                "color: %2;"
                "font: 15px;"

            "}"
            "QMenu::item:selected {"
                "border-color: %3;"
                "background: %3;"
            "}"
        ).arg(p_set->mColors[5], p_set->mColors[1], p_set->mColors[18]);

    p_menu->setStyleSheet(menu_style);

    //Create find action
    QAction* p_find_action = p_menu->addAction(tr("&Find"));
    p_find_action->setShortcuts({QKeySequence("Ctrl+F")});

    //Execute the menuand control wether or not is find action
    if(p_find_action == p_menu->exec(e->globalPos())){

        emit OpenSearchWidget(textCursor().selectedText());
    }

    delete p_find_action;
    delete p_menu;
}

void CSingleEditor::wheelEvent(QWheelEvent *e)
{
    if (e->modifiers() & Qt::ControlModifier) {
        const int delta = e->delta();
        if (delta < 0)
            zoomOut();
        else if (delta > 0)
            zoomIn();

        updateLineNumberArea();
        return;
    }
    QTextEdit::wheelEvent(e);
}


void CSingleEditor::resizeEvent(QResizeEvent *e)
{
    QTextEdit::resizeEvent(e);

    QRect cr = this->contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), lineNumberAreaWidth(), cr.height()));
}

void CSingleEditor::ControlContentChange()
{
    QString new_content = toPlainText();

    if(new_content != mCurrentContent){

        emit ContentChanged();

        mCurrentContent = new_content;
    }
}


int CSingleEditor::getFirstVisibleBlockId()
{
    // Detect the first block for which bounding rect - once translated
    // in absolute coordinated - is contained by the editor's text area

    // Costly way of doing but since "blockBoundingGeometry(...)" doesn't
    // exists for "QTextEdit"...

    QTextCursor curs = QTextCursor(this->document());
    curs.movePosition(QTextCursor::Start);
    for(int i=0; i < this->document()->blockCount(); ++i)
    {
        QTextBlock block = curs.block();

        QRect r1 = this->viewport()->geometry();
        QRect r2 = this->document()->documentLayout()->blockBoundingRect(block).translated(
                    this->viewport()->geometry().x(), this->viewport()->geometry().y() - (
                        this->verticalScrollBar()->sliderPosition()
                        ) ).toRect();

        if (r1.contains(r2, true)) { return i; }

        curs.movePosition(QTextCursor::NextBlock);
    }

    return 0;
}

void CSingleEditor::lineNumberAreaPaintEvent(QPaintEvent *event)
{
    CStartupSettings* p_set = CStartupSettings::GetInstance();;

    this->verticalScrollBar()->setSliderPosition(this->verticalScrollBar()->sliderPosition());

    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), p_set->mColors[0]);
    int blockNumber = this->getFirstVisibleBlockId();

    QTextBlock block = this->document()->findBlockByNumber(blockNumber);
    QTextBlock prev_block = (blockNumber > 0) ? this->document()->findBlockByNumber(blockNumber-1) : block;
    int translate_y = (blockNumber > 0) ? -this->verticalScrollBar()->sliderPosition() : 0;

    int top = this->viewport()->geometry().top();

    // Adjust text position according to the previous "non entirely visible" block
    // if applicable. Also takes in consideration the document's margin offset.
    int additional_margin;
    if (blockNumber == 0)
        // Simply adjust to document's margin
        additional_margin = (int) this->document()->documentMargin() -1 - this->verticalScrollBar()->sliderPosition();
    else
        // Getting the height of the visible part of the previous "non entirely visible" block
        additional_margin = (int) this->document()->documentLayout()->blockBoundingRect(prev_block)
                .translated(0, translate_y).intersected(this->viewport()->geometry()).height();

    // Shift the starting point
    top += additional_margin;

    int bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();

    //QColor col_1(90, 255, 30);      // Current line (custom green)
    //QColor col_0(120, 120, 120);    // Other lines  (custom darkgrey)

    QColor col_1(p_set->mColors[5]);      // Current line
    QColor col_0(p_set->mColors[14]);    // Other lines

    // Draw the numbers (displaying the current line number in green)
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(QColor(120, 120, 120));
            painter.setPen((this->textCursor().blockNumber() == blockNumber) ? col_1 : col_0);
            painter.drawText(-5, top,
                             lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignRight, number);
        }

        block = block.next();
        top = bottom;
        bottom = top + (int) this->document()->documentLayout()->blockBoundingRect(block).height();
        ++blockNumber;
    }

}
