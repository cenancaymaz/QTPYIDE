#include "code_editor.h"
#include <QBoxLayout>
#include <QSettings>


CCodeEditor::CCodeEditor(QWidget *parent)
    : QFrame{parent}
{
    //This part is for Windows Dark Theme users
    #ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
        if(settings.value("AppsUseLightTheme")==0){
           setStyleSheet("CCodeEditor{ border: 1px solid ""#FF4C4C4C""; }");
        }else{

            setStyleSheet("CCodeEditor{ border: 1px solid ligthgray; }");
        }
    #endif

    pTabWidget = new QTabWidget(this);
    pTabWidget->setMovable(true);
    LatestTabNo = 0;

    pRunButton = new QPushButton(tr("Run"), this);
    connect(pRunButton, &QPushButton::clicked, this, &CCodeEditor::SendInput);

    pRunSelectedButton = new QPushButton(tr("Run Selected"), this);
    connect(pRunSelectedButton, &QPushButton::clicked, this, &CCodeEditor::SendSelected);

    QVBoxLayout *vl = new QVBoxLayout();
    QHBoxLayout *hl = new QHBoxLayout();

    setLayout(vl);

    vl->addWidget(pTabWidget);

    hl->addWidget(pRunButton);
    hl->addWidget(pRunSelectedButton);

    vl->addLayout(hl);


    AddTab();
    AddTab();
}

QTextEdit *CCodeEditor::CreateAnEditor()
{
    //Create an empty Editor
    QTextEdit* p_text_edit = new QTextEdit(this);

    return p_text_edit;
}

QTextEdit *CCodeEditor::AddTab(QFileInfo FileInfo)
{
    if(!FileInfo.fileName().isEmpty() || !FileInfo.fileName().isEmpty()){//if the tab created by selecting a file fro file view

        if(mTabInfos.value(FileInfo.filePath())){//if there is a tab with the same file info

            qDebug()<<"Same occurred";
            return NULL;
        }

    }


    QTextEdit *tab = CreateAnEditor();

    if(FileInfo.fileName().isEmpty()){
        FileInfo.setFile(QString("File%1").arg(++LatestTabNo));
    }
    pTabWidget->insertTab(pTabWidget->count(), tab, FileInfo.fileName());

    //For Close Button
    QToolButton *cb = new QToolButton();
    cb->setText("x");
    cb->setStyleSheet("border : none");
    connect(cb, &QToolButton::clicked, this, &CCodeEditor::CloseTab);

    pTabWidget->tabBar()->setTabButton(pTabWidget->count() - 1, QTabBar::RightSide, cb);

    pTabWidget->setCurrentIndex(pTabWidget->count() - 1);


    //Add the new tab to tab info map
    mTabInfos.insert(FileInfo.filePath(), tab);
    return tab;
}

void CCodeEditor::WriteFiletoTab(QTextEdit *Tab, QString Path)
{
    QFile file(Path);
    if(file.open(QIODevice::ReadOnly)) {

        Tab->setText(file.readAll());
        file.close();

    }else{

        qDebug()<<"File cannot be open.";

    }

}

void CCodeEditor::OpenFile(QFileInfo FileInfo)
{
    QTextEdit* p_tab = AddTab(FileInfo);

    if(p_tab){//A new tab is created

        WriteFiletoTab(p_tab, FileInfo.filePath());

    }
}

void CCodeEditor::CloseTab()
{
    QToolButton* p_tool_button = qobject_cast<QToolButton*>(sender());
    QTabBar* p_tab_bar = qobject_cast<QTabBar*>(sender()->parent());

    int closed_ind = p_tab_bar->tabAt(p_tool_button->pos());

    pTabWidget->removeTab(closed_ind);

    if(closed_ind > 0){

        pTabWidget->setCurrentIndex(closed_ind - 1);
    }

    if(pTabWidget->count() == 0){

        pRunButton->setEnabled(false);
        pRunSelectedButton->setEnabled(false);
    }
}

void CCodeEditor::SendInput()
{
    //Take selected tab
    QTextEdit* p_text_edit = qobject_cast<QTextEdit*>(pTabWidget->currentWidget());

    if(!p_text_edit){//There is no text edit in the tab widget
        return;
    }

    QString s = p_text_edit->toPlainText();

    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

void CCodeEditor::SendSelected()
{
    //Take selected tab
    QTextEdit* p_text_edit = qobject_cast<QTextEdit*>(pTabWidget->currentWidget());

    if(!p_text_edit){//There is no text edit in the tab widget
        return;
    }

    QString s = p_text_edit->textCursor().selectedText();

    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

