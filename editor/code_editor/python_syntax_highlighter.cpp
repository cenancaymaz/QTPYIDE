#include "python_syntax_highlighter.h"
#include <QDebug>
#include <QDir>

#include "../../util/startup_settings.h"
#include "../console_view/python_process.h"

#ifdef Q_OS_WIN
    #include <windows.h>
#endif

PythonSyntaxHighlighter::PythonSyntaxHighlighter(QFileInfo FileInfo, QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    pDoc = qobject_cast<QTextDocument*>(parent);

    connect(pDoc, &QTextDocument::contentsChange, this, &PythonSyntaxHighlighter::SaveLastContentChange);

    LastPos = 0;
    LastRem = 0;
    LastAdd = 0;
    isConCh = false;


    pSyntaxControlTimer = new QTimer(this);
    pSyntaxControlTimer->setInterval(3000);
    connect(pSyntaxControlTimer, &QTimer::timeout, this, &PythonSyntaxHighlighter::SyntaxControl);
    pSyntaxControlTimer->start();


    mFileInfo = FileInfo;

    keywords = QStringList() << "and" << "assert" << "break" << "class" << "continue" << "def" <<
                                "del" << "elif" << "else" << "except" << "exec" << "finally" <<
                                "for" << "from" << "global" << "if" << "import" << "in" <<
                                "is" << "lambda" << "not" << "or" << "pass" << "print" <<
                                "raise" << "return" << "try" << "while" << "yield" <<
                                "None" << "True" << "False";

    operators = QStringList() << "=" <<
                                 // Comparison
                                 "==" << "!=" << "<" << "<=" << ">" << ">=" <<
                                 // Arithmetic
                                 "\\+" << "-" << "\\*" << "/" << "//" << "%" << "\\*\\*" <<
                                 // In-place
                                 "\\+=" << "-=" << "\\*=" << "/=" << "%=" <<
                                 // Bitwise
                                 "\\^" << "\\|" << "&" << "~" << ">>" << "<<";

    braces = QStringList() << "{" << "}" << "\\(" << "\\)" << "\\[" << "]";

    CStartupSettings* p_set = GetStartupSettings();

    basicStyles.insert("keyword", getTextCharFormat(p_set->mColors[11]));//green
    basicStyles.insert("operator", getTextCharFormat(p_set->mColors[8]));//red
    basicStyles.insert("brace", getTextCharFormat(p_set->mColors[5]));//base1
    basicStyles.insert("defclass", getTextCharFormat(p_set->mColors[12], "bold"));//blue
    basicStyles.insert("brace", getTextCharFormat(p_set->mColors[5]));//base1
    basicStyles.insert("string", getTextCharFormat(p_set->mColors[9]));//cyan1
    basicStyles.insert("string2", getTextCharFormat(p_set->mColors[13]));// cyan2
    basicStyles.insert("comment", getTextCharFormat(p_set->mColors[14], "italic"));//base01
    basicStyles.insert("self", getTextCharFormat(p_set->mColors[15], "italic"));//yellow
    basicStyles.insert("numbers", getTextCharFormat(p_set->mColors[16]));//violet
    basicStyles.insert("func", getTextCharFormat(p_set->mColors[17]));//magenta


    triSingleQuote.setPattern("'''");
    triDoubleQuote.setPattern("\"\"\"");

    initializeRules();
}

PythonSyntaxHighlighter::~PythonSyntaxHighlighter()
{
    //syn_hello.cpython-310.pyc
    //Delete temporary script file
    DeleteSynFile(mFileInfo.dir().path() + "/syn_" + mFileInfo.fileName());
    //Delete temporary compiled file
    //Python version control will be added
    DeleteSynFile(mFileInfo.dir().path() + "/__pycache__/syn_" + mFileInfo.fileName().remove(".py") + ".cpython-310.pyc");
}



void PythonSyntaxHighlighter::initializeRules()
{
    //function usage
    rules.append(HighlightingRule("\\.[^\\(]*", 0, basicStyles.value("func")));

    foreach (QString currKeyword, keywords)
    {
        rules.append(HighlightingRule(QString("\\b%1\\b").arg(currKeyword), 0, basicStyles.value("keyword")));
    }
    foreach (QString currOperator, operators)
    {
        rules.append(HighlightingRule(QString("%1").arg(currOperator), 0, basicStyles.value("operator")));
    }
    foreach (QString currBrace, braces)
    {
        rules.append(HighlightingRule(QString("%1").arg(currBrace), 0, basicStyles.value("brace")));
    }


    // 'self'
    rules.append(HighlightingRule("\\bself\\b", 0, basicStyles.value("self")));

    // Double-quoted string, possibly containing escape sequences
    // FF: originally in python : r'"[^"\\]*(\\.[^"\\]*)*"'
    rules.append(HighlightingRule("\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"", 0, basicStyles.value("string")));
    // Single-quoted string, possibly containing escape sequences
    // FF: originally in python : r"'[^'\\]*(\\.[^'\\]*)*'"
    rules.append(HighlightingRule("'[^'\\\\]*(\\\\.[^'\\\\]*)*'", 0, basicStyles.value("string")));

    // 'def' followed by an identifier
    // FF: originally: r'\bdef\b\s*(\w+)'
    rules.append(HighlightingRule("\\bdef\\b\\s*(\\w+)", 1, basicStyles.value("defclass")));
    //  'class' followed by an identifier
    // FF: originally: r'\bclass\b\s*(\w+)'
    rules.append(HighlightingRule("\\bclass\\b\\s*(\\w+)", 1, basicStyles.value("defclass")));



    // From '#' until a newline
    // FF: originally: r'#[^\\n]*'
    rules.append(HighlightingRule("#[^\\n]*", 0, basicStyles.value("comment")));

    // Numeric literals
    rules.append(HighlightingRule("\\b[+-]?[0-9]+[lL]?\\b", 0, basicStyles.value("numbers"))); // r'\b[+-]?[0-9]+[lL]?\b'
    rules.append(HighlightingRule("\\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\\b", 0, basicStyles.value("numbers"))); // r'\b[+-]?0[xX][0-9A-Fa-f]+[lL]?\b'
    rules.append(HighlightingRule("\\b[+-]?[0-9]+(?:\\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\\b", 0, basicStyles.value("numbers"))); // r'\b[+-]?[0-9]+(?:\.[0-9]+)?(?:[eE][+-]?[0-9]+)?\b'





}

void PythonSyntaxHighlighter::highlightBlock(const QString &text)
{
    foreach (HighlightingRule currRule, rules)
    {
        int idx = currRule.pattern.indexIn(text, 0);
        while (idx >= 0)
        {
            // Get index of Nth match
            idx = currRule.pattern.pos(currRule.nth);
            int length = currRule.pattern.cap(currRule.nth).length();
            setFormat(idx, length, currRule.format);
            idx = currRule.pattern.indexIn(text, idx + length);
        }
    }

    setCurrentBlockState(0);

    // Do multi-line strings
    bool isInMultilne = matchMultiline(text, triSingleQuote, 1, basicStyles.value("string2"));
    if (!isInMultilne)
        matchMultiline(text, triDoubleQuote, 2, basicStyles.value("string2"));


    QTextCharFormat char_format;
    char_format.setUnderlineStyle(QTextCharFormat::SingleUnderline);
    char_format.setUnderlineColor(QColor(255,0,0));
    char_format.setFontUnderline(true);

    //SyntaxControl();

    if(!mSyntaxControlReply.isEmpty()){

        qDebug()<<"There is error:";

        QString err_str = mSyntaxControlReply.join("\n");

        qDebug().noquote()<<err_str;

        qDebug()<<"Text: "<<text;


        if (mSyntaxControlReply[1].toUtf8() == text) {

            // The following line does not work !
            char_format.setToolTip("Hasan");


            qDebug()<<"Error catched!";
            setFormat(0, text.length(), char_format);
        }
    }
}

bool PythonSyntaxHighlighter::matchMultiline(const QString &text, const QRegExp &delimiter, const int inState, const QTextCharFormat &style)
{
    int start = -1;
    int add = -1;
    int end = -1;
    int length = 0;

    // If inside triple-single quotes, start at 0
    if (previousBlockState() == inState) {
        start = 0;
        add = 0;
    }
    // Otherwise, look for the delimiter on this line
    else {
        start = delimiter.indexIn(text);
        // Move past this match
        add = delimiter.matchedLength();
    }

    // As long as there's a delimiter match on this line...
    while (start >= 0) {
        // Look for the ending delimiter
        end = delimiter.indexIn(text, start + add);
        // Ending delimiter on this line?
        if (end >= add) {
            length = end - start + add + delimiter.matchedLength();
            setCurrentBlockState(0);
        }
        // No; multi-line string
        else {
            setCurrentBlockState(inState);
            length = text.length() - start + add;
        }
        // Apply formatting and look for next
        setFormat(start, length, style);
        start = delimiter.indexIn(text, start + length);
    }
    // Return True if still inside a multi-line string, False otherwise
    if (currentBlockState() == inState)
        return true;
    else
        return false;
}

const QTextCharFormat PythonSyntaxHighlighter::getTextCharFormat(const QString &colorName, const QString &style)
{
    QTextCharFormat charFormat;
    QColor color(colorName);
    charFormat.setForeground(color);
    if (style.contains("bold", Qt::CaseInsensitive))
        charFormat.setFontWeight(QFont::Bold);
    if (style.contains("italic", Qt::CaseInsensitive))
        charFormat.setFontItalic(true);

    return charFormat;
}


void PythonSyntaxHighlighter::SyntaxControl()
{
    //Create syn-file path
    QString syn_file_path = mFileInfo.dir().path() + "/syn_" + mFileInfo.fileName();

    //Create syn-file
    CreateSynFile(syn_file_path);

    //Control syn-file syntax
    //Clear reply
    mSyntaxControlReply.clear();
    //Create process
    CPythonProcess* p_python_process = new CPythonProcess(this);
    //Connect process reply to reply receiving
    connect(p_python_process, &CPythonProcess::OnErrOut, this, &PythonSyntaxHighlighter::SyntaxControlReceive);
    p_python_process->ControlSyntax(syn_file_path);

    //pDoc = qobject_cast<QTextDocument*>(parent());

    if(isConCh){
        pDoc->contentsChange(LastPos, LastRem, LastAdd);
    }


    delete p_python_process;
}

void PythonSyntaxHighlighter::SyntaxControlReceive(QString ReplyLine)
{
    mSyntaxControlReply.append(ReplyLine);
}


void PythonSyntaxHighlighter::CreateSynFile(QString FilePath)
{
    //QTextDocument* p_text_document = qobject_cast<QTextDocument*>(parent());


    QFile file(FilePath);

    if(file.open(QIODevice::ReadWrite)) {

        //clear the contents of the file
        file.resize(0);

        //Create a text stream
        QTextStream stream(&file);

        //Save original file editor text to syn-file
        stream<< pDoc->toPlainText().toUtf8();
        //close the file
        file.close();

    }

    //set syn-file as hidden
    #ifdef Q_OS_WIN
        SetFileAttributesA(FilePath.toStdString().c_str(), FILE_ATTRIBUTE_HIDDEN);
    #endif
}

void PythonSyntaxHighlighter::DeleteSynFile(QString FilePath)
{
    QFile file(FilePath);
    file.remove();
}

void PythonSyntaxHighlighter::SaveLastContentChange(int position, int charsRemoved, int charsAdded)
{
    LastPos = position;
    LastRem = charsRemoved;
    LastAdd = charsAdded;
    isConCh = true;
}


