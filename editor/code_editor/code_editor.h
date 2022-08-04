#ifndef CTEXTEDITOR_H
#define CTEXTEDITOR_H

#include "../../util/borderless_button.h"
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

    QFrame* pSearchWidget;
    QLineEdit* pSearchEdit;
    CBorderlessButton* pFindPrevButton;
    CBorderlessButton* pFindNextButton;
    CBorderlessButton* pCancelSearchButton;
    void CreateSearchWidget();

    QMap<QString, CSingleEditor*> mTabInfos;// Path or Designeted name for new file - Tab
    QVector<PythonSyntaxHighlighter*> mHighligtherVector; //it is here just for clearing the false positive meamory leak warning
    CSingleEditor* AddTab(QFileInfo FileInfo = QFileInfo());

    void WriteFiletoTab(CSingleEditor* Tab, QString Path);

public slots:

    void OpenFile(QFileInfo FileInfo);
    void OpenSearchWidget(QString SelectedText);

    void SaveFile();
    void SaveAsFile();
    void SendInput();
    void SendSelected();

private slots:

    void ControlContentChange();
    void CloseTab();

    void FindPrevButtonClicked();
    void FindNextButtonClicked();
    void CancelSearchButtonClicked();

signals:
    void InputEntered(QString text);
    void EnableButtons(bool enable);

};

#endif // CTEXTEDITOR_H
