#include "borderless_button.h"
#include "startup_settings.h"

CBorderlessButton::CBorderlessButton(QString text, QWidget *parent)
    : QPushButton{text, parent}
{
    SetStyle();
}

CBorderlessButton::CBorderlessButton(QIcon icon, QWidget *parent)
    : QPushButton{icon, "", parent}
{
    SetStyle();
}

void CBorderlessButton::SetStyle()
{
    CStartupSettings* p_set = GetStartupSettings();

    //Create a borderless button stylesheet
    QString sty = QString("QPushButton{"
                              "border: 4px solid %1;"
                          "}"
                          "QPushButton:hover {"
                              "background-color: %2;"
                              "border: 1px solid %3;"
                          "}"
                          "QPushButton:pressed {"
                              "background-color: %3;"
                              "border: 4px solid %3;"
                          "}").arg(p_set->mColors[19], p_set->mColors[20], p_set->mColors[21]);
    setStyleSheet(sty);
    p_set->SettoDefaultFontSize(this);
}
