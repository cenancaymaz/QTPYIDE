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

    QPushButton* pRunButton;
    QPushButton* pRunSelectedButton;


    QMap<QString, QTextEdit*> mTabInfos;// Path - Tab
    QTextEdit* AddTab(QFileInfo FileInfo = QFileInfo());

    void WriteFiletoTab(QTextEdit* Tab, QString Path);

public slots:

    void OpenFile(QFileInfo FileInfo);

private slots:

    void CloseTab();


    void SendInput();
    void SendSelected();

signals:
    void InputEntered(QString text);

};

#endif // CTEXTEDITOR_H
