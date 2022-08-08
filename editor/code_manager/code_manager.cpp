#include "code_manager.h"

#include <QBoxLayout>

#include "../../util/startup_settings.h"


CCodeManager::CCodeManager(QWidget *parent)
    : QFrame{parent}
{
    CStartupSettings* p_set = CStartupSettings::GetInstance();;

    //for border style
    setStyleSheet(QString("CCodeManager{ border: 1px solid %1; }").arg(p_set->mColors[10]));

    //This widget should have fixed height
    setFixedHeight(40);

    CreateCurrentFilePathLabel();
    CreateSaveButton();
    CreateSaveAsButton();
    CreateRunSelectedButton();
    CreateRunButton();

    //start code managers buttons as disabled
    EnableButtons(false);

    QHBoxLayout *hl = new QHBoxLayout(this);
    hl->setContentsMargins(3, 3, 3, 3);

    hl->addWidget(pCurrentFilePathLabel);

    hl->addStretch(1);

    hl->addWidget(pSaveButton);
    hl->addWidget(pSaveAsButton);
    hl->addWidget(pRunSelectedButton);
    hl->addWidget(pRunButton);

}

void CCodeManager::CreateCurrentFilePathLabel()
{
    CStartupSettings* p_set = CStartupSettings::GetInstance();;
    pCurrentFilePathLabel = new QLabel("Code Manager", this);
    p_set->SettoDefaultFontSize(pCurrentFilePathLabel);
}

void CCodeManager::CreateSaveButton()
{
    pSaveButton = new CBorderlessButton(QIcon(":/icons/save-icon"), this);
    connect(pSaveButton, &CBorderlessButton::clicked, this, &CCodeManager::SaveFile);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+S"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeManager::SaveFile);

    pSaveButton->setToolTip(tr("Save - Ctrl+S"));

    pSaveButton->setFixedSize(this->height() - 6, this->height() - 6);
    pSaveButton->setIconSize(QSize(pSaveButton->width() - 1, pSaveButton->height() - 1));
}

void CCodeManager::CreateSaveAsButton()
{
    pSaveAsButton = new CBorderlessButton(QIcon(":/icons/save-as-icon"), this);
    connect(pSaveAsButton, &CBorderlessButton::clicked, this, &CCodeManager::SaveAsFile);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+Shift+S"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeManager::SaveAsFile);

    pSaveAsButton->setToolTip(tr("Save As - Ctrl+Shift+S"));
    pSaveAsButton->setFixedSize(this->height() - 6, this->height() - 6);
    pSaveAsButton->setIconSize(QSize(pSaveAsButton->width() - 1, pSaveAsButton->height() - 1));
}

void CCodeManager::CreateRunSelectedButton()
{
    pRunSelectedButton = new CBorderlessButton(QIcon(":/icons/run-selected-icon"), this);
    connect(pRunSelectedButton, &CBorderlessButton::clicked, this, &CCodeManager::SendSelected);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("F9"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeManager::SendSelected);

    pRunSelectedButton->setToolTip(tr("Run Selected - F9"));
    pRunSelectedButton->setFixedSize(this->height() - 6, this->height() - 6);
    pRunSelectedButton->setIconSize(QSize(pRunSelectedButton->width() - 1, pRunSelectedButton->height() - 1));

}

void CCodeManager::CreateRunButton()
{
    pRunButton = new CBorderlessButton(QIcon(":/icons/run-icon"), this);
    connect(pRunButton, &CBorderlessButton::clicked, this, &CCodeManager::SendInput);

    QShortcut *p_shortcut = new QShortcut(QKeySequence("Ctrl+R"), this);
    QObject::connect(p_shortcut, &QShortcut::activated, this, &CCodeManager::SendInput);

    pRunButton->setToolTip(tr("Run - Ctrl+R"));
    pRunButton->setFixedSize(this->height() - 6, this->height() - 6);
    pRunButton->setIconSize(QSize(pRunButton->width() - 1, pRunButton->height() - 1));
}

void CCodeManager::EnableButtons(bool enable)
{
    pSaveButton->setEnabled(enable);
    pSaveAsButton->setEnabled(enable);
    pRunSelectedButton->setEnabled(enable);
    pRunButton->setEnabled(enable);
}

void CCodeManager::CurrentFilePathChanged(QString Path)
{
    pCurrentFilePathLabel->setText(Path.mid(Path.indexOf(mWorkingDir)));
}

void CCodeManager::WorkingDirChanged(QString DirName)
{
    mWorkingDir = DirName;
}
