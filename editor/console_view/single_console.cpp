#include "single_console.h"
#include "../../startup_settings.h"

CSingleConsole::CSingleConsole(QWidget *parent)
    : QTextEdit{parent}
{
    setReadOnly(true);

    pPythonProcess = new CPythonProcess(this);

    connect(pPythonProcess, &CPythonProcess::OnStdOut, this, &CSingleConsole::OnStdOut);
    connect(pPythonProcess, &CPythonProcess::OnErrOut, this, &CSingleConsole::OnErrOut);

    //Initialize and insert input indicator
    CStartupSettings* p_set = GetStartupSettings();
    mInputIndCol = p_set->mColors[6];
    InsertInputInd();
}

void CSingleConsole::StartProcess(QString Script)
{
    pPythonProcess->StartProcess(Script);
}

void CSingleConsole::WriteInput(QString Inp)
{

    CStartupSettings* p_set = GetStartupSettings();

    //Set cursor to the end of the edit
    QTextCursor new_cursor = textCursor();
    new_cursor.movePosition(QTextCursor::End);
    setTextCursor(new_cursor);

    QStringList lines = Inp.split("\n");

    //for the first line, dont put "..."
    //Write the input
    setTextColor(p_set->mColors[2]);
    insertPlainText(QString(lines[0] + "\n"));

    for(int i = 1; i < lines.length(); i++){
        //Put input indicator
        setTextColor(p_set->mColors[6]);
        insertPlainText(".  .  . ");
        //Write the input
        setTextColor(p_set->mColors[2]);
        insertPlainText(QString(lines[i] + "\n"));
    }
    insertPlainText("\n");

    InsertInputInd();
    //Set scrollbar to the bottom
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());


    //Send the text to python process
    pPythonProcess->WriteInp(Inp);

}

void CSingleConsole::InsertInputInd()
{
    setTextColor(mInputIndCol);
    insertPlainText(">>> ");
}

void CSingleConsole::DeleteLastLine()
{
    QTextCursor store_cursor_pos = textCursor();
    moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
    moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
    moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
    textCursor().removeSelectedText();
    textCursor().deletePreviousChar();
    setTextCursor(store_cursor_pos);
}

void CSingleConsole::OnStdOut(QString Out)
{
    //Delete Input Ind
    DeleteLastLine();

    CStartupSettings* p_set = GetStartupSettings();

    //Set cursor to the end of the edit
    QTextCursor new_cursor = textCursor();
    new_cursor.movePosition(QTextCursor::End);
    setTextCursor(new_cursor);


    //Add the output text with appropriate color
    setTextColor(p_set->mColors[2]);
    insertPlainText(Out);
    insertPlainText("\n\n");

    InsertInputInd();
    //Set scrollbar to the bottom
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());
}

void CSingleConsole::OnErrOut(QString Err)
{
    //Delete Input Ind
    DeleteLastLine();

    CStartupSettings* p_set = GetStartupSettings();

    //Set cursor to the end of the edit
    QTextCursor new_cursor = textCursor();
    new_cursor.movePosition(QTextCursor::End);
    setTextCursor(new_cursor);


    //Add the output text with appropriate color
    setTextColor(p_set->mColors[6]);
    insertPlainText(Err);
    insertPlainText("\n\n");

    InsertInputInd();
    //Set scrollbar to the bottom
    verticalScrollBar()->setValue(verticalScrollBar()->maximum());

}
