#ifndef CTEXTEDITOR_H
#define CTEXTEDITOR_H

#include <QtWidgets>

class CCodeEditor : public QFrame
{
    Q_OBJECT
public:
    explicit CCodeEditor(QWidget *parent = nullptr);

private:

    int LatestTabNo;

    QTabWidget* pTabWidget;
    QTextEdit *CreateAnEditor();

    QPushButton* pSaveButton;
    QPushButton* pSaveAsButton;
    QPushButton* pRunButton;
    QPushButton* pRunSelectedButton;


    QMap<QString, QTextEdit*> mTabInfos;// Path or Designeted name for new file - Tab
    QTextEdit* AddTab(QFileInfo FileInfo = QFileInfo());

    void WriteFiletoTab(QTextEdit* Tab, QString Path);

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
