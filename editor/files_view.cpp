#include "files_view.h"

CFilesView::CFilesView(QWidget *parent)
    : QFrame{parent}
{
    //This part is for Windows Dark Theme users
    #ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
        if(settings.value("AppsUseLightTheme")==0){
           setStyleSheet("CFilesView{ border: 1px solid ""#FF4C4C4C""; }");
        }else{

            setStyleSheet("CFilesView{ border: 1px solid ligthgray; }");
        }
    #endif

    CreateTree();
    QVBoxLayout* vl = new QVBoxLayout(this);

    vl->addWidget(pTree);

}

void CFilesView::CreateTree()
{
    pTree = new QTreeView(this);

//Create Model
    pModel = new QFileSystemModel(this);
    pModel->setRootPath(QDir::currentPath());
    pModel->setFilter(QDir::AllDirs | QDir::AllEntries |QDir::NoDotAndDotDot);

    //Give a name filter to model an set the filtered out hidden
    QStringList name_filters;
    name_filters << "*.txt";
    pModel->setNameFilters(name_filters);
    pModel->setNameFilterDisables(false);


    pTree->setModel(pModel);
    //Path of the root of tree view
    pTree->setRootIndex(pModel->index(QDir::currentPath()));

    //Hide size and type collumns
    pTree->hideColumn(1);
    pTree->hideColumn(2);


    //For collumn stretch
    pTree->header()->setSectionResizeMode(0, QHeaderView::Interactive);
    pTree->header()->resizeSection(0, 200);
    //pTree->header()->setSectionResizeMode(1, QHeaderView::Stretch);
    pTree->header()->resizeSection(3, 150);
    //pTree->header()->setSectionResizeMode(1, QHeaderView::Fixed);
    //pTree->header()->setStretchLastSection(false);


    connect(pTree, &QTreeView::doubleClicked, this, &CFilesView::TreeDoubleClicked);

}

void CFilesView::TreeDoubleClicked(const QModelIndex &index)
{
    QFileInfo file_info = pModel->fileInfo(index);

    if(file_info.isFile()){
        qDebug()<<pModel->fileInfo(index).absoluteFilePath();
        emit FileSelected(pModel->fileInfo(index));
    }
}


