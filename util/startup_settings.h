#ifndef CSTARTUPSETTINGS_H
#define CSTARTUPSETTINGS_H

#include <QObject>
#include <QVector>
#include <QWidget>


class CStartupSettings : public QObject
{
    Q_OBJECT
public:
    explicit CStartupSettings(QObject *parent = nullptr);

    void SettoDefaultFontSize(QWidget *Widget);

    QVector<QString> mColors;

    bool mIsDarkMode = false;

private:

    int mDefaultFontSize;


signals:

};


static CStartupSettings* pStartupSettingInstance = 0;

[[maybe_unused]]static CStartupSettings* GetStartupSettings(){

    if(!pStartupSettingInstance){

        pStartupSettingInstance = new CStartupSettings();

    }

    return pStartupSettingInstance;
};


#endif // CSTARTUPSETTINGS_H
