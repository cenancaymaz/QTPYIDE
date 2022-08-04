#include "files_view.h"
#include "../../util/startup_settings.h"

CFilesView::CFilesView(QWidget *parent)
    : QFrame{parent}
{   
    CStartupSettings* p_set = GetStartupSettings();

    setStyleSheet(QString("CFilesView{ border: 1px solid %1; }").arg(p_set->mColors[10]));

    //Initial dir
    mWorkingPath = QDir::currentPath();
    mWorkingDir = QDir::current().dirName();


    //For setting the font of the Frame

    p_set->SettoDefaultFontSize(this);

//    QFont font = this->font();
//    font.setPointSize(9);
//    this->setFont(font);

    QLabel* p_label = new QLabel("Working Dir: ", this);
    p_set->SettoDefaultFontSize(p_label);
    QFont label_font = p_label->font();
    label_font.setBold(true);
    p_label->setFont(label_font);

    CreateWorkingDirButton();
    CreateTree();

    QVBoxLayout* vl = new QVBoxLayout(this);
    vl->setContentsMargins(3, 3, 3, 3);
    QHBoxLayout* hl = new QHBoxLayout();
    hl->addWidget(p_label);
    hl->addWidget(pWorkingDirButton);
    hl->addStretch(1);
    vl->addLayout(hl);
    vl->addWidget(pTree);

}

QString CFilesView::GetWorkingPath()
{
    return mWorkingPath;
}

void CFilesView::CreateWorkingDirButton()
{
    pWorkingDirButton = new QPushButton(this);
    pWorkingDirButton->setFont(this->font());

    connect(pWorkingDirButton, &QPushButton::clicked, this, &CFilesView::OnWorkingDirButtonClicked);

    //For first opening
    pWorkingDirButton->setText(mWorkingDir);
    pWorkingDirButton->setToolTip(mWorkingPath);

}

void CFilesView::CreateTree()
{
    pTree = new QTreeView(this);

    //Create Model
    pModel = new QFileSystemModel(this);
    pModel->setRootPath(mWorkingPath);
    pModel->setFilter(QDir::AllDirs | QDir::AllEntries |QDir::NoDotAndDotDot);

    //Give a name filter to model an set the filtered out hidden
    QStringList name_filters;
    name_filters <<"*.py";
    pModel->setNameFilters(name_filters);
    pModel->setNameFilterDisables(false);


    pTree->setModel(pModel);
    //Path of the root of tree view
    pTree->setRootIndex(pModel->index(mWorkingPath));

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


    //For file doubleclick mechanism
    connect(pTree, &QTreeView::doubleClicked, this, &CFilesView::TreeDoubleClicked);

    //For setting the font of the tree  
    CStartupSettings* p_set = GetStartupSettings();
    p_set->SettoDefaultFontSize(pTree);
    p_set->SettoDefaultFontSize(pTree->header());

    //For building rigth click mechanism
    BuildContextMenu();


}

void CFilesView::BuildContextMenu()
{
    //Build Context Menu Open mechanism
    pTree->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(pTree, &QTreeView::customContextMenuRequested, this, &CFilesView::OnCustomContextMenu);


    //Create Context Menus
    pFileContextMenu = new QMenu(pTree);
    pDirContextMenu = new QMenu(pTree);

    //Add Actions to the menus

    //New File Action is for file and directory
    pNewFileAction = new QAction(tr("New File"),this);
    pNewFileAction->setStatusTip("Create a new file");
    pFileContextMenu->addAction(pNewFileAction);
    pDirContextMenu->addAction(pNewFileAction);
    connect(pNewFileAction, &QAction::triggered, this, &CFilesView::NewFileActionTriggered);


    //Delete Action is just for file
    pDeleteAction = new QAction(tr("Delete"),this);
    pDeleteAction->setStatusTip("Delete the file");
    pFileContextMenu->addAction(pDeleteAction);
    connect(pDeleteAction, &QAction::triggered, this, &CFilesView::DeleteActionTriggered);

}

void CFilesView::OnWorkingDirButtonClicked()
{
    //Select the new directory
    QString path = QFileDialog::getExistingDirectory(this, tr("Choose a directory."), mWorkingPath, QFileDialog::ShowDirsOnly);

    if(path.isEmpty()) return; //Directory is not selected

    mWorkingPath = path;
    mWorkingDir = QDir(mWorkingPath).dirName();

    emit WorkingPathChanged(mWorkingPath);

    //Set current dir button
    pWorkingDirButton->setText(mWorkingDir);
    pWorkingDirButton->setToolTip(mWorkingPath);

    //Set tree to selected path
    pModel->setRootPath(mWorkingPath);
    pTree->setRootIndex(pModel->index(mWorkingPath));

}

void CFilesView::TreeDoubleClicked(const QModelIndex &index)
{
    QFileInfo file_info = pModel->fileInfo(index);

    if(file_info.isFile()){
        emit FileSelected(pModel->fileInfo(index));
    }
}

void CFilesView::OnCustomContextMenu(const QPoint &point)
{
    mMenuPoint = point;
    //Find the info of right clicked item
    QFileInfo file_info = pModel->fileInfo(pTree->indexAt(mMenuPoint));

    //if it is not exists, open a dir context menu
    //if it is a file, open filecontext menu
    //if it is a dir, open dir context menu

    if(!file_info.exists()){

        pDirContextMenu->exec(pTree->viewport()->mapToGlobal(point));

    }else if(file_info.isFile()){

        pFileContextMenu->exec(pTree->viewport()->mapToGlobal(point));

    }else if(file_info.isDir()){

        pDirContextMenu->exec(pTree->viewport()->mapToGlobal(point));

    }
}

void CFilesView::DeleteActionTriggered()
{
    //Find the info of right clicked item
    QFileInfo file_info = pModel->fileInfo(pTree->indexAt(mMenuPoint));

    if(file_info.isFile()){//Control whether it is a file

        //Ask user whether they are sure
        QMessageBox::StandardButton control = QMessageBox::question(this, "", tr("Are you sure to delete %1?").arg(file_info.fileName()),
                                        QMessageBox::Yes|QMessageBox::No);
        if (control == QMessageBox::Yes) {
            //Remove the file
            QFile file (file_info.absoluteFilePath());
            file.remove();
        }

    }else{

        //It is not a file
        QMessageBox msg_box;
        msg_box.setIcon(QMessageBox::Critical);
        msg_box.setText("The file has a problem.");
        msg_box.exec();

    }

}

void CFilesView::NewFileActionTriggered()
{
    qDebug()<<"New File triggered";


    //Take new file name from user
    bool ok;
    QString file_name = QInputDialog::getText(this, tr("New File"),
                                         tr("File name:"), QLineEdit::Normal,
                                         "", &ok, Qt::MSWindowsFixedSizeDialogHint);
    if (!ok || file_name.isEmpty()){

        //User cancels the new file creation
        return;
    }

    //Find the info of right clicked item
    QFileInfo file_info = pModel->fileInfo(pTree->indexAt(mMenuPoint));

    //Find the directory path that is right clicked
    QString dir_path;

    if(!file_info.exists()){

        dir_path = pModel->rootPath();

    }else if(file_info.isFile()){//it is a file so find the dir taht contains the file
        dir_path = file_info.dir().absolutePath();
    }else if(file_info.isDir()){//it is a dir so use directly
        dir_path = file_info.absoluteFilePath();
    }else{//Not suppossed to happen

        return;
    }

    //Create file path for the new file
    QString file_path = dir_path + "/" + file_name.remove(".py") + ".py";

    //Create the file with QFile and open
    QFile file(file_path);

    //Control existance of the file
    if(file.exists()){

        //File already exists
        QMessageBox msg_box;
        msg_box.setIcon(QMessageBox::Critical);
        msg_box.setText("The file already exists.");
        msg_box.exec();
        return;
    }

    if(file.open(QIODevice::ReadWrite)) {

        //After opening, allways close
        file.close();
        //Open the tab on code editor
        emit FileSelected(QFileInfo(file));
    }else{

        //The file cannot be created
        QMessageBox msg_box;
        msg_box.setIcon(QMessageBox::Critical);
        msg_box.setText("The file cannot be created.");
        msg_box.exec();

    }
}


