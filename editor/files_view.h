#ifndef CFILESVIEW_H
#define CFILESVIEW_H

#include <QtWidgets>

class CFilesView : public QFrame
{
    Q_OBJECT
public:
    explicit CFilesView(QWidget *parent = nullptr);

private:
    QTreeView *pTree;
    QFileSystemModel *pModel;

    void CreateTree();

private slots:
    void TreeDoubleClicked(const QModelIndex &index);

signals:

    void FileSelected(QFileInfo FileInfo);

};

#endif // FILES_VIEW_H
