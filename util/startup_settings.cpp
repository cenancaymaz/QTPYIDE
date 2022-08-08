#include "startup_settings.h"

#include <QSettings>
#include "python_process.h"
#include <QDebug>


CStartupSettings::CStartupSettings(QObject *parent)
    : QObject{parent}
{
    //Get the python version of the system
    CPythonProcess* p_python_process = new CPythonProcess(this);
    mPythonVersion = p_python_process->GetPythonVersion();
    delete p_python_process;

    mDefaultFontSize = 9;

    #ifdef Q_OS_WIN
        QSettings settings("HKEY_CURRENT_USER\\Software\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize",QSettings::NativeFormat);
        if(settings.value("AppsUseLightTheme")==0){

            mIsDarkMode = true;

        }
    #endif

    mColors = QVector<QString>(22,"");

    if(mIsDarkMode){
        //Dark solarized
        mColors.insert(0, "#073642");//base02
        mColors.insert(1, "#646464");//Disable color
        mColors.insert(2, "#839496");//base0
        mColors.insert(3, "#002b36");//base03
        mColors.insert(4, "#ffffff");//Qt::white
        mColors.insert(5, "#93a1a1");//base1
        mColors.insert(6, "#ff0000");//Qt::red
        mColors.insert(7, "#2a82da");//Link color
        mColors.insert(8, "#dc322f");//red
        mColors.insert(9, "#2aa198");//cyan1
        mColors.insert(10, "#4c4c4c");//border color
        mColors.insert(11, "#859900");//green
        mColors.insert(12, "#268bd2");//blue
        mColors.insert(13, "#2a84a2");//cyan2
        mColors.insert(14, "#586e75");//base01
        mColors.insert(15, "#b58900");//yellow
        mColors.insert(16, "#6c71c4");//violet
        mColors.insert(17, "#d33682");//magenta
        mColors.insert(18, "#dc322f");//red select

        //borderless button colors
        mColors.insert(19, "#073642");//base02 - backround color
        mColors.insert(20, "#1c4954");//base02/light - hover color
        mColors.insert(21, "#dc322f");//base03 - press color
    }else{
        //Light solarized
        mColors.insert(0, "#eee8d5");//base2--
        mColors.insert(1, "#aaaaaa");//Disable color--
        mColors.insert(2, "#657b83");//base00--
        mColors.insert(3, "#fdf6e3");//base3--
        mColors.insert(4, "#ffffff");//Qt::white --
        mColors.insert(5, "#586e75");//base01 --
        mColors.insert(6, "#ff0000");//Qt::red --
        mColors.insert(7, "#2a82da");//Link color --
        mColors.insert(8, "#dc322f");//red --
        mColors.insert(9, "#2aa198");//cyan1 --
        mColors.insert(10, "#aeadac");//border color
        mColors.insert(11, "#859900");//green --
        mColors.insert(12, "#268bd2");//blue --
        mColors.insert(13, "#2a84a2");//cyan2 --
        mColors.insert(14, "#93a1a1");//base1 --
        mColors.insert(15, "#b58900");//yellow --
        mColors.insert(16, "#6c71c4");//violet --
        mColors.insert(17, "#d33682");//magenta --
        mColors.insert(18, "#eba0a0");//red select

        //borderless button colors
        mColors.insert(19, "#eee8d5");//base2 - backround color
        mColors.insert(20, "#fbf6e8");//base2/light - hover color
        mColors.insert(21, "#eba0a0");//base03 - press color
    }

}

CStartupSettings *CStartupSettings::GetInstance()
{
    static CStartupSettings* p_instance = new CStartupSettings();
    return p_instance;
}

void CStartupSettings::SettoDefaultFontSize(QWidget *Widget)
{
    QFont font = Widget->font();
    font.setPointSize(mDefaultFontSize);
    Widget->setFont(font);
}


