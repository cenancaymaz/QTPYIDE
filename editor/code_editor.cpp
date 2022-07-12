#include "code_editor.h"
#include <QBoxLayout>
#include <QSettings>


CCodeEditor::CCodeEditor(QWidget *parent)
    : QFrame{parent}
{
    //setTitle(tr("Code Editor"));

    //This part is for Windows Dark Theme users
    #ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
        if(settings.value("AppsUseLightTheme")==0){
           setStyleSheet("CCodeEditor{ border: 1px solid ""#FF4C4C4C""; }");
        }else{

            setStyleSheet("CCodeEditor{ border: 1px solid ligthgray; }");
        }
    #endif

    pTextEdit = new QTextEdit(this);

    pRunButton = new QPushButton(tr("Run"), this);
    connect(pRunButton, &QPushButton::clicked, this, &CCodeEditor::SendInput);

    pRunSelectedButton = new QPushButton(tr("Run Selected"), this);
    connect(pRunSelectedButton, &QPushButton::clicked, this, &CCodeEditor::SendSelected);

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();

    setLayout(vl);

    vl->addWidget(pTextEdit);

    hl->addWidget(pRunButton);
    hl->addWidget(pRunSelectedButton);

    vl->addLayout(hl);
}

void CCodeEditor::SendInput()
{
    QString s = pTextEdit->toPlainText();

    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

void CCodeEditor::SendSelected()
{
    QString s = pTextEdit->textCursor().selectedText();

    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

