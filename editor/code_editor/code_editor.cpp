#include "code_editor.h"

#include "../../startup_settings.h"


CCodeEditor::CCodeEditor(QWidget *parent)
    : QFrame{parent}
{
    CStartupSettings* p_set = GetStartupSettings();

    setStyleSheet(QString("CCodeEditor{ border: 1px solid %1; }").arg(p_set->mColors[10]));


    pTabWidget = new QTabWidget(this);
    pTabWidget->setMovable(true);

    p_set->SettoDefaultFontSize(pTabWidget);


    LatestTabNo = 0;

    CreateSaveButton();
    CreateSaveAsButton();
    CreateRunButton();
    CreateRunSelectedButton();

    //Disable buttons in opening
    EnableButtons(false);


    QVBoxLayout *vl = new QVBoxLayout();
    QGridLayout *gl = new QGridLayout();

    setLayout(vl);

    vl->addWidget(pTabWidget);

    gl->addWidget(pSaveButton, 0, 0);
    gl->addWidget(pSaveAsButton, 0, 1);
    gl->addWidget(pRunSelectedButton, 0, 2);
    gl->addWidget(pRunButton, 0, 3);


    vl->addLayout(gl);

}

CSingleEditor *CCodeEditor::CreateAnEditor(QFileInfo FileInfo)
{
    //Create an empty Editor
    CSingleEditor* p_text_edit = new CSingleEditor(this);

    PythonSyntaxHighlighter *p_python_highlighter = new PythonSyntaxHighlighter(FileInfo, p_text_edit->document());

    mHighligtherVector.append(p_python_highlighter);


    return p_text_edit;
}

void CCodeEditor::CreateSaveButton()
{
    pSaveButton = new QPushButton(tr("Save"), this);
    connect(pSaveButton, &QPushButton::clicked, this, &CCodeEditor::SaveFile);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeEditor::SaveFile);

    pSaveButton->setToolTip(tr("Ctrl + S"));
}

void CCodeEditor::CreateSaveAsButton()
{
    pSaveAsButton = new QPushButton(tr("Save As"), this);
    connect(pSaveAsButton, &QPushButton::clicked, this, &CCodeEditor::SaveAsFile);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeEditor::SaveAsFile);

    pSaveAsButton->setToolTip(tr("Ctrl + Shift + S"));
}

void CCodeEditor::CreateRunButton()
{
    pRunButton = new QPushButton(tr("Run"), this);
    connect(pRunButton, &QPushButton::clicked, this, &CCodeEditor::SendInput);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeEditor::SendInput);

    pRunButton->setToolTip(tr("Ctrl + R"));
}

void CCodeEditor::CreateRunSelectedButton()
{
    pRunSelectedButton = new QPushButton(tr("Run Selected"), this);
    connect(pRunSelectedButton, &QPushButton::clicked, this, &CCodeEditor::SendSelected);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("F9"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeEditor::SendSelected);

    pRunSelectedButton->setToolTip(tr("F9"));
}

void CCodeEditor::EnableButtons(bool enable)
{
    pSaveButton->setEnabled(enable);
    pSaveAsButton->setEnabled(enable);
    pRunButton->setEnabled(enable);
    pRunSelectedButton->setEnabled(enable);
}

CSingleEditor *CCodeEditor::AddTab(QFileInfo FileInfo)
{
    if(!FileInfo.fileName().isEmpty() || !FileInfo.fileName().isEmpty()){//if the tab created by selecting a file from file view

        if(mTabInfos.value(FileInfo.filePath())){//if there is a tab with the same file info

            return NULL;
        }

    }
    //qDebug()<<FileInfo.filePath();

    //Create a tab
    CSingleEditor *tab = CreateAnEditor(FileInfo);

    if(FileInfo.fileName().isEmpty()){//A new file that is created by user
        FileInfo.setFile(QString("File%1").arg(++LatestTabNo));
    }

    //Insert tab to tab widget
    pTabWidget->insertTab(pTabWidget->count(), tab, FileInfo.fileName());

    //Insert tab tool tip to the tab
    pTabWidget->setTabToolTip(pTabWidget->count()-1, "Path: " + FileInfo.filePath());


    //Create Close Button and attach it to the tab
    QToolButton *cb = new QToolButton();
    cb->setText("x");
    cb->setStyleSheet("border : none");
    CStartupSettings* p_set = GetStartupSettings();
    p_set->SettoDefaultFontSize(cb);
    connect(cb, &QToolButton::clicked, this, &CCodeEditor::CloseTab);
    pTabWidget->tabBar()->setTabButton(pTabWidget->count() - 1, QTabBar::RightSide, cb);

    //Set current index to created tab
    pTabWidget->setCurrentIndex(pTabWidget->count() - 1);

    //Add the new tab to tab info map
    mTabInfos.insert(FileInfo.filePath(), tab);

    //Enable buttons in case of buttons are disabled by no tab situation
    EnableButtons(true);

    return tab;
}

void CCodeEditor::WriteFiletoTab(CSingleEditor *Tab, QString Path)
{
    QFile file(Path);
    if(file.open(QIODevice::ReadOnly)) {

        QTextStream stream(&file);
        QString data = stream.readAll();

        Tab->SetCurrentContent(data.toUtf8());
        //Tab->setText(data.toUtf8());
        file.close();

    }else{

        qDebug()<<"File cannot be open.";

    }

}

void CCodeEditor::OpenFile(QFileInfo FileInfo)
{
    CSingleEditor* p_tab = AddTab(FileInfo);

    if(p_tab){//A new tab is created

        //Write text from the file to the tab
        WriteFiletoTab(p_tab, FileInfo.filePath());

        //connect tab's content change to content change control slot
        connect(p_tab, &CSingleEditor::ContentChanged, this, &CCodeEditor::ControlContentChange);


    }else{//The tab is already created. Open this tab

        pTabWidget->setCurrentWidget(mTabInfos.value(FileInfo.filePath()));
    }
}

void CCodeEditor::ControlContentChange()
{
    //Take the current tabs name
    QString name = pTabWidget->tabText(pTabWidget->currentIndex());

    //If it is not marked as "changed", mark as "changed" with *
    if(name.right(1) != "*"){//The last char

        name.append("*");
        pTabWidget->setTabText(pTabWidget->currentIndex(), name);
    }
}

void CCodeEditor::CloseTab()
{
    //Take the sender close button
    QToolButton* p_tool_button = qobject_cast<QToolButton*>(sender());

    //Find the tab index of the button
    int closed_ind = pTabWidget->tabBar()->tabAt(p_tool_button->pos());

    //Take the tab
    CSingleEditor* p_tab = qobject_cast<CSingleEditor*>(pTabWidget->widget(closed_ind));

    //Remove tab from tab infos
    QString key = mTabInfos.key(p_tab);
    mTabInfos.remove(key);

    //Remove tab from tabwidget
    pTabWidget->removeTab(closed_ind);

    //Delete the tab
    delete p_tab;

    //set the current tab to prev tab from removed
    if(closed_ind > 0){

        pTabWidget->setCurrentIndex(closed_ind - 1);
    }

    //if there is no tab, disable buttons
    if(pTabWidget->count() == 0){

        EnableButtons(false);
    }

}

void CCodeEditor::SaveFile()
{
    //Take the selected tab
    CSingleEditor* p_tab = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

    //Find the path of the tab
    QString path = mTabInfos.key(p_tab);

    QString tab_name = pTabWidget->tabText(pTabWidget->currentIndex());
    QString tab_name_wo_change = tab_name;
    tab_name_wo_change.chop(1);//to clear *



    if(tab_name == path || tab_name_wo_change == path){
        //if tab name and path is the same, this tab is newly created, not opened.
        //it should be redirected to SaveAs
        SaveAsFile();
        return;
    }

    //Open the file
    QFile file(path);
    if(file.open(QIODevice::ReadWrite)) {

        //clear the contents of the file
        file.resize(0);
        //Create a text stream
        QTextStream stream(&file);

        //Write text to the strean
        stream<< p_tab->toPlainText().toUtf8();

        //close the file
        file.close();



        //Take the current tabs name
        QString name = pTabWidget->tabText(pTabWidget->currentIndex());

        //If it is marked as "changed", delete the "changed" mark *
        if(name.right(1) == "*"){//The last char

            name.chop(1);
            pTabWidget->setTabText(pTabWidget->currentIndex(), name);
        }

    }else{

        qDebug()<<"File cannot be open.";

    }
}

void CCodeEditor::SaveAsFile()
{
    //File select dialog
    QString path = QFileDialog::getSaveFileName(this, tr("Save File"),
                               "",
                               tr("Text files (*.py)"));


    //A file name selected
    if(!path.isEmpty()){

        //Control wether the tab of this path already opened.
        if(mTabInfos.value(path)){//if there is a tab with the same file info

            QMessageBox msg_box;
            msg_box.setIcon(QMessageBox::Critical);
            msg_box.setText("Cannot Save. The document is already opened in different tab.");
            msg_box.exec();
            return;
        }

        //Open the file
        QFile file(path);
        if(file.open(QIODevice::ReadWrite)) {

            //clear the contents of the file
            file.resize(0);

            //Create a text stream
            QTextStream stream(&file);

            //Take the selected tab
            CSingleEditor* p_tab = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

            //Write text to the stream
            stream<< p_tab->toPlainText().toUtf8();

            //Change the tab name
            QFileInfo file_info(file);
            pTabWidget->setTabText(pTabWidget->currentIndex(), file_info.fileName());

            //Change the tab path
            QString key = mTabInfos.key(p_tab); //First, remove the tab from map
            mTabInfos.remove(key);
            mTabInfos.insert(file_info.filePath(), p_tab);//Then, add tab with new key(path)


            //close the file
            file.close();

        }else{

            qDebug()<<"File cannot be open.";

        }


    }
}

void CCodeEditor::SendInput()
{
    //Save the file
    SaveFile();

    //Take selected tab
    CSingleEditor* p_text_edit = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

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
    CSingleEditor* p_text_edit = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

    if(!p_text_edit){//There is no text edit in the tab widget
        return;
    }

    QString s = p_text_edit->textCursor().selection().toPlainText();

    qDebug()<<s;
    //s.replace("\n", "\r\n");


    if(!s.isEmpty()){
        emit InputEntered(s);
    }
}

