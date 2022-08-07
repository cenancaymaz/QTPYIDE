#ifndef CFILESVIEW_H
#define CFILESVIEW_H

#include <QtWidgets>

class CFilesView : public QFrame
{
    Q_OBJECT
public:
    explicit CFilesView(QWidget *parent = nullptr);

    QString GetWorkingPath();
    QString GetWorkingDir();

private:

    QString mWorkingPath;
    QString mWorkingDir;

    QPushButton *pWorkingDirButton;
    void CreateWorkingDirButton();

    QTreeView *pTree;
    QFileSystemModel *pModel;

    QMenu* pFileContextMenu;
    QMenu* pDirContextMenu;

    QPoint mMenuPoint;

    QAction* pDeleteAction;
    QAction* pNewFileAction;

    void CreateTree();
    void BuildContextMenu();

private slots:

    void OnWorkingDirButtonClicked();

    void TreeDoubleClicked(const QModelIndex &index);
    void OnCustomContextMenu(const QPoint &point);

    //Context Menu Action slots
    void DeleteActionTriggered();
    void NewFileActionTriggered();

signals:

    void FileSelected(QFileInfo FileInfo);
    void WorkingPathChanged(QString Path);
    void WorkingDirChanged(QString Path);
};

#endif // FILES_VIEW_H
