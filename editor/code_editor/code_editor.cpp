#include "code_editor.h"

#include "../../util/startup_settings.h"



CCodeEditor::CCodeEditor(QWidget *parent)
    : QFrame{parent}
{
    CStartupSettings* p_set = GetStartupSettings();

    setStyleSheet(QString("CCodeEditor{ border: 1px solid %1; }").arg(p_set->mColors[10]));


    pTabWidget = new QTabWidget(this);
    pTabWidget->setMovable(true);

    p_set->SettoDefaultFontSize(pTabWidget);
    LatestTabNo = 0;

    CreateSearchWidget();

    QVBoxLayout *vl = new QVBoxLayout(this);
    vl->setContentsMargins(3, 3, 3, 3);

    vl->addWidget(pTabWidget);
    vl->addWidget(pSearchWidget);

}

CSingleEditor *CCodeEditor::CreateAnEditor(QFileInfo FileInfo)
{
    //Create an empty Editor
    CSingleEditor* p_text_edit = new CSingleEditor(this);

    //Create Python highligter and bound it to the Single editor
    PythonSyntaxHighlighter *p_python_highlighter = new PythonSyntaxHighlighter(FileInfo, p_text_edit->document());

    //Add CTRL-F funcionality
    connect(p_text_edit, &CSingleEditor::OpenSearchWidget, this, &CCodeEditor::OpenSearchWidget);

    mHighligtherVector.append(p_python_highlighter);


    return p_text_edit;
}

void CCodeEditor::CreateSearchWidget()
{
    CStartupSettings* p_set = GetStartupSettings();

    //Create the search widget and set its border
    pSearchWidget = new QFrame(this);
    pSearchWidget->setObjectName("SearchWidget");
    pSearchWidget->setStyleSheet(QString("QFrame#SearchWidget{ border: 1px solid %1; }").arg(p_set->mColors[10]));

    //Create the label
    QLabel* p_label = new QLabel(tr("Find:"), pSearchWidget);

    //Create search edit and connect its enter press signal to find next
    pSearchEdit = new QLineEdit(pSearchWidget);
    connect(pSearchEdit, &QLineEdit::returnPressed, this, &CCodeEditor::FindNextButtonClicked);

    //Create find prev button and connect its clicked signal to find prev
    pFindPrevButton = new CBorderlessButton(tr("Find Previous  "), pSearchWidget);
    connect(pFindPrevButton, &CBorderlessButton::clicked, this, &CCodeEditor::FindPrevButtonClicked);

    //Create find next button and connect its clicked signal to find next
    pFindNextButton = new CBorderlessButton(tr("Find Next  "), pSearchWidget);
    connect(pFindNextButton, &CBorderlessButton::clicked, this, &CCodeEditor::FindNextButtonClicked);

    //Create cancel search button and connect its clicked signal to slot that used for closing(setting invis) of the search widget
    pCancelSearchButton = new CBorderlessButton(" x ", pSearchWidget);
    connect(pCancelSearchButton, &CBorderlessButton::clicked, this, &CCodeEditor::CancelSearchButtonClicked);

    //Set fonts of the buttons to default font size
    p_set->SettoDefaultFontSize(p_label);


    QHBoxLayout* hl = new QHBoxLayout(pSearchWidget);
    hl->setContentsMargins(3, 3, 3, 3);

    hl->addWidget(p_label);
    hl->addWidget(pSearchEdit, 6); //This is for bigger search box
    hl->addWidget(pFindPrevButton);
    hl->addWidget(pFindNextButton);

    hl->addStretch(1);
    hl->addWidget(pCancelSearchButton);


    //Start with invisible Search Widget
    pSearchWidget->setVisible(false);

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
    emit EnableButtons(true);

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

void CCodeEditor::OpenSearchWidget(QString SelectedText)
{
    //Open search widget
    pSearchWidget->setVisible(true);
    //Set the text of searched to the selected text
    pSearchEdit->setText(SelectedText);
    //Focus on search edit
    pSearchEdit->setFocus();
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

        emit EnableButtons(false);
    }

}

void CCodeEditor::FindPrevButtonClicked()
{
    //Take selected tab
    CSingleEditor* p_text_edit = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

    if(p_text_edit){//Control wether or not there is an editor is shown
        p_text_edit->FindPrev(pSearchEdit->text());
    }
}

void CCodeEditor::FindNextButtonClicked()
{
    //Take selected tab
    CSingleEditor* p_text_edit = qobject_cast<CSingleEditor*>(pTabWidget->currentWidget());

    if(p_text_edit){//Control wether or not there is an editor is shown
        p_text_edit->FindNext(pSearchEdit->text());
    }
}

void CCodeEditor::CancelSearchButtonClicked()
{
    pSearchWidget->setVisible(false);
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

