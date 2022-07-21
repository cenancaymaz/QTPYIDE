#ifndef CTEXTEDITOR_H
#define CTEXTEDITOR_H

#include "python_syntax_highlighter.h"
#include "single_editor.h"

#include <QtWidgets>
class CCodeEditor : public QFrame
{
    Q_OBJECT
public:
    explicit CCodeEditor(QWidget *parent = nullptr);

private:

    int LatestTabNo;

    QTabWidget* pTabWidget;
    CSingleEditor *CreateAnEditor(QFileInfo FileInfo);

    QPushButton* pSaveButton;
    void CreateSaveButton();
    QPushButton* pSaveAsButton;
    void CreateSaveAsButton();
    QPushButton* pRunButton;
    void CreateRunButton();
    QPushButton* pRunSelectedButton;
    void CreateRunSelectedButton();

    void EnableButtons(bool enable = true);


    QMap<QString, CSingleEditor*> mTabInfos;// Path or Designeted name for new file - Tab
    QVector<PythonSyntaxHighlighter*> mHighligtherVector; //it is here just for clearing the false positive meamory leak warning
    CSingleEditor* AddTab(QFileInfo FileInfo = QFileInfo());

    void WriteFiletoTab(CSingleEditor* Tab, QString Path);

public slots:

    void OpenFile(QFileInfo FileInfo);

private slots:

    void ControlTextChange();
    void CloseTab();

    void SaveFile();
    void SaveAsFile();
    void SendInput();
    void SendSelected();

signals:
    void InputEntered(QString text);

};

#endif // CTEXTEDITOR_H
