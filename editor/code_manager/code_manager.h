#ifndef CCODEMANAGER_H
#define CCODEMANAGER_H

#include <QtWidgets>
#include "../../util/borderless_button.h"

class CCodeManager : public QFrame
{
    Q_OBJECT
public:
    explicit CCodeManager(QWidget *parent = nullptr);

private:

    QLabel* pCurrentFilePathLabel;
    QString mWorkingDir;

    void CreateCurrentFilePathLabel();
    CBorderlessButton* pSaveButton;
    void CreateSaveButton();
    CBorderlessButton* pSaveAsButton;
    void CreateSaveAsButton();
    CBorderlessButton* pRunSelectedButton;
    void CreateRunSelectedButton();
    CBorderlessButton* pRunButton;
    void CreateRunButton();


public slots:
    void EnableButtons(bool enable);
    void CurrentFilePathChanged(QString Path);
    void WorkingDirChanged(QString DirName);

signals:
    void SaveFile();
    void SaveAsFile();
    void SendSelected();
    void SendInput();


};

#endif // CCODEMANAGER_H
