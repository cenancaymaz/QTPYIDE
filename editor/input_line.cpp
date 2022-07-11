#include "input_line.h"
#include <QLabel>
#include <QSettings>

#include <QHBoxLayout>

CInputLine::CInputLine(QWidget *parent)
    : QFrame{parent}
{    
    //setFrameStyle(QFrame::Box);

//This part is for Windows Dark Theme users
#ifdef Q_OS_WIN
    QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
    if(settings.value("AppsUseLightTheme")==0){
       setStyleSheet("CInputLine{ border: 1px solid ""#FF4C4C4C""; }");
    }else{

        setStyleSheet("CInputLine{ border: 1px solid ligthgray; }");
    }
#endif


    pInputEdit = new QLineEdit(this);
    connect(pInputEdit, &QLineEdit::returnPressed, this, &CInputLine::SendInput);

    pSendButton = new QPushButton(">", this);
    pSendButton->setFixedWidth(30);
    connect(pSendButton, &QPushButton::clicked, this, &CInputLine::SendInput);


    QHBoxLayout* hl = new QHBoxLayout(this);

    hl->addWidget(new QLabel(tr("Input:"), this));
    hl->addWidget(pInputEdit);
    hl->addWidget(pSendButton);
}

void CInputLine::SendInput()
{
    pInputEdit->setFocus();
    if(!pInputEdit->text().isEmpty()){
        emit InputEntered(pInputEdit->text());
        pInputEdit->clear();
    }
}
