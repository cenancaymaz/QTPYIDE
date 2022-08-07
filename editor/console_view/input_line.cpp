#include "input_line.h"

CInputLine::CInputLine(QWidget *parent)
    : QWidget{parent}
{    
    //Create Edit
    pInputEdit = new QLineEdit(this);
    //Connect enter pressed to Input Sending
    connect(pInputEdit, &QLineEdit::returnPressed, this, &CInputLine::SendInput);
    //CEvent filter for up and down pressing for history browsing
    pInputEdit->installEventFilter(this);
    mLastPressed = UpButton;


    //Create Button and connect it to Input Sending
    pSendButton = new QPushButton(">", this);
    pSendButton->setFixedWidth(30);
    connect(pSendButton, &QPushButton::clicked, this, &CInputLine::SendInput);

    //Layouting
    QHBoxLayout* hl = new QHBoxLayout(this);
    hl->addWidget(new QLabel(tr("In:"), this));
    hl->addWidget(pInputEdit);
    hl->addWidget(pSendButton);
}

bool CInputLine::eventFilter(QObject *obj, QEvent *e)
{
    if (obj == pInputEdit)
        {
            if (e->type() == QEvent::KeyPress)
            {
                QKeyEvent* keyEvent = static_cast<QKeyEvent*>(e);
                if (keyEvent->key() == Qt::Key_Up)
                {
                     UpPressed();
                     return true;
                }
                else if(keyEvent->key() == Qt::Key_Down)
                {
                    DownPressed();
                    return true;
                }
            }
            return false;
        }
        return QWidget::eventFilter(obj, e);
}

void CInputLine::UpPressed()
{
    //If the main stack is not empty
    if(!mHistoryStack.empty()){
        //If last time pressed is down, you should pop 2 times
        if(mLastPressed == DownButton){
            mHistoryStack2.push(mHistoryStack.pop());
            mLastPressed = UpButton;
            UpPressed();
        }else{
            //Take last element from the main stack
            QString old = mHistoryStack.pop();
            //Add it to the secondary stack
            mHistoryStack2.push(old);
            //Write the text to the edit
            pInputEdit->setText(old);
            //Record last pressed
            mLastPressed = UpButton;
        }
    }
}

void CInputLine::DownPressed()
{
    //If the secondary stack is not empty
    if(!mHistoryStack2.empty()){
        //If last time pressed is up, you should pop 2 times
        if(mLastPressed == UpButton){
            mHistoryStack.push(mHistoryStack2.pop());
            mLastPressed = DownButton;
            DownPressed();
        }else{
            //take last element from the secondary stack
            QString old = mHistoryStack2.pop();
            //Add it to the main stack
            mHistoryStack.push(old);
            //Write the text to the edit
            pInputEdit->setText(old);
            //Record last pressed
            mLastPressed = DownButton;
        }
    }else{
        //If there is no text in the secondary stack, clear the input edit
        pInputEdit->setText("");
        //For pressing mechanism, you should record it as Up
        mLastPressed = UpButton;
    }
}

void CInputLine::AddToHistoryStack(QString Text)
{
    //First, reset the history stacks
    while(!mHistoryStack2.empty()){
        mHistoryStack.push(mHistoryStack2.pop());
    }
    //Add new item to history stack
    mHistoryStack.push(Text);
    //For pressed mechanisms
    mLastPressed = UpButton;

    //Limit the stack size for the memory
    if(mHistoryStack.size() > 10){

        mHistoryStack.removeFirst();

    }
}

void CInputLine::SendInput()
{
    //If it is sent by button, focus it to the edit
    pInputEdit->setFocus();
    //Sent if only edit is not empty
    if(!pInputEdit->text().isEmpty()){

//        //Add it to the history stacks
//        AddToHistoryStack(pInputEdit->text());

        //Send it
        emit InputSent(pInputEdit->text());
        //clear edit after sending
        pInputEdit->clear();
    }
}


