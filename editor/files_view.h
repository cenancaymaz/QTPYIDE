#ifndef CFILESVIEW_H
#define CFILESVIEW_H

#include <QtWidgets>

class CFilesView : public QFrame
{
    Q_OBJECT
public:
    explicit CFilesView(QWidget *parent = nullptr);

private:

    QString mCurrentPath;
    QString mCurrentDir;

    QPushButton *pCurrentDirButton;
    void CreateCurrentDirButton();

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

    void OnCurrentDirButtonClicked();

    void TreeDoubleClicked(const QModelIndex &index);
    void OnCustomContextMenu(const QPoint &point);

    //Context Menu Action slots
    void DeleteActionTriggered();
    void NewFileActionTriggered();


signals:

    void FileSelected(QFileInfo FileInfo);

};

#endif // FILES_VIEW_H
