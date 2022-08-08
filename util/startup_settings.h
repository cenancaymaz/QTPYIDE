#ifndef CSTARTUPSETTINGS_H
#define CSTARTUPSETTINGS_H

#include <QObject>
#include <QVector>
#include <QWidget>


class CStartupSettings : public QObject
{
    Q_OBJECT
public:

    /// Static getter
    static CStartupSettings* GetInstance();

    void SettoDefaultFontSize(QWidget *Widget);

    QVector<QString> mColors;

    bool mIsDarkMode = false;

    QString mPythonVersion;

private:

    explicit CStartupSettings(QObject *parent = nullptr);

    int mDefaultFontSize;


signals:

};


#endif // CSTARTUPSETTINGS_H
